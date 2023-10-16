// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include <tango/tango.h>

#include <ChimeraTK/BackendRegisterCatalogue.h>
#include <ChimeraTK/DeviceBackend.h>
#include <ChimeraTK/DeviceBackendImpl.h>

namespace Tango {
  class DeviceProxy;
}

namespace ChimeraTK {

  class TangoBackend;

  class TangoBackendRegisterAccessorBase {
   public:
    TangoBackendRegisterAccessorBase(std::shared_ptr<TangoBackend> backend, TangoBackendRegisterInfo* info)
    : _backend(backend), _info(info) {}

   private:
    std::shared_ptr<TangoBackend> _backend{};
    TangoBackendRegisterInfo* _info{nullptr};
  };

  class TangoBackendRegisterInfo : public BackendRegisterInfoBase {
   public:
    TangoBackendRegisterInfo(const Tango::AttributeInfoEx& attribute);
    TangoBackendRegisterInfo() = default;
    ~TangoBackendRegisterInfo() final = default;

    /** Return full path name of the register (including modules) */
    [[nodiscard]] RegisterPath getRegisterName() const final;

    /** Return number of elements per channel */
    [[nodiscard]] unsigned int getNumberOfElements() const final;

    /** Return number of channels in register */
    [[nodiscard]] unsigned int getNumberOfChannels() const final;

    /** Return desciption of the actual payload data for this register. See the
     * description of DataDescriptor for more information. */
    [[nodiscard]] const DataDescriptor& getDataDescriptor() const final;

    /** Return whether the register is readable. */
    [[nodiscard]] bool isReadable() const final;

    /** Return whether the register is writeable. */
    [[nodiscard]] bool isWriteable() const final;

    /** Return all supported AccessModes for this register */
    [[nodiscard]] AccessModeFlags getSupportedAccessModes() const final;

    /** Create copy of the object */
    [[nodiscard]] std::unique_ptr<BackendRegisterInfoBase> clone() const final {
      return std::make_unique<TangoBackendRegisterInfo>(*this);
    }

    [[nodiscard]] int getTangoType() const { return _tangoType; }

   private:
    RegisterPath _name;
    size_t _nElements;
    size_t _nChannels;
    bool _readable{false};
    bool _writeable{false};
    DataDescriptor _descriptor;
    int _tangoType{0};
  };

  template<typename UserType>
  class TangoBackendRegisterAccessor : public TangoBackendRegisterAccessorBase, public NDRegisterAccessor<UserType> {
   public:
    TangoBackendRegisterAccessor(const RegisterPath& path, std::shared_ptr<TangoBackend> backend,
        TangoBackendRegisterInfo* info, AccessModeFlags flags, size_t numberOfWords, size_t wordOffsetInRegister)
    : TangoBackendRegisterAccessorBase(backend, info), NDRegisterAccessor<UserType>(path, flags) {
      NDRegisterAccessor<UserType>::buffer_2D.resize(info->getNumberOfChannels());
      for(size_t y = 0; y < info->getNumberOfChannels(); y++) {
        this.acessChannel(y).resize(numberOfWords);
      }

      _isPartial = _info->getNumberOfElements() != numberOfWords;
      NDRegisterAccessor<UserType>::_exceptionBackend = backend;
    }
    void doReadTransferSynchronously() override;

    bool doWriteTransfer(VersionNumber) override {
      write_internal();
      return false;
    }

    void doPreRead(TransferType) override {
      if(!_backend->isOpen()) throw ChimeraTK::logic_error("Read operation not allowed while device is closed.");
      if(!isReadable()) throw ChimeraTK::logic_error("Try to read from write-only register \"" + _path + "\".");
    }

    void doPreWrite(TransferType, VersionNumber) override {
      if(!_backend->isOpen()) throw ChimeraTK::logic_error("Write operation not allowed while device is closed.");
      if(!isWriteable()) throw ChimeraTK::logic_error("Try to write read-only register \"" + _path + "\".");
    }

    void doPostRead(TransferType, bool hasNewData) override {
      if(!hasNewData) return;

      // Note: the original idea was to extract the time stamp from the received data. This idea has been dropped since
      // the time stamp attached to the data seems to be unreliably, at least for the x2timer macro pulse number. If the
      // unreliable time stamp is attached to the trigger, all data will get this time stamp. This leads to error
      // messages of the DOOCS history archiver, which rejects data due to wrong time stamps. Hence we better generate
      // our own time stamp here.

      // See spec. B.1.3.4.2
      TransferElement::setDataValidity(
          _attribute.get_quality() != Tango::AttrQuality::ATTR_VALID ? DataValidity::ok : DataValidity::faulty);

      auto type = getTargetDataType(typeid(UserType));
      callForType(type, [&](auto T) {});
    }

    bool isReadOnly() const override { return isReadable() && not isWriteable(); }

    bool isReadable() const override { return _info->isReadable(); }

    bool isWriteable() const override { return _info->isWriteable(); }

    bool mayReplaceOther(const boost::shared_ptr<TransferElement const>& other) const override {
      auto rhsCasted = boost::dynamic_pointer_cast<const DoocsBackendRegisterAccessor<UserType>>(other);
      if(!rhsCasted) return false;
      if(_path != rhsCasted->_path) return false;
      if(nElements != rhsCasted->nElements) return false;
      if(elementOffset != rhsCasted->elementOffset) return false;
      return true;
    }

    std::vector<boost::shared_ptr<TransferElement>> getHardwareAccessingElements() override {
      return {boost::enable_shared_from_this<TransferElement>::shared_from_this()};
    }

    std::list<boost::shared_ptr<ChimeraTK::TransferElement>> getInternalElements() override { return {}; }

    void replaceTransferElement(boost::shared_ptr<TransferElement> /*newElement*/) override {} // LCOV_EXCL_LINE

    void interrupt() override { this->interrupt_impl(this->notifications); }

    bool _isPartial{false};
    Tango::DeviceAttribute _attribute{};
  };

  template<typename UserType>
  void TangoBackendRegisterAccessor<UserType>::doReadTransferSynchronously() {
    if(!_backend->isFunctional()) {
      throw ChimeraTK::runtime_error(std::string("Exception reported by another accessor."));
    }

    try {
      _attribute = std::move(_backend->_deviceProxy->read_attribute(_info->_name));
    }
    catch(...) {
      _backend->informRuntimeError(_path);
      throw ChimeraTK::runtime_error(std::string("Cannot read Tango attribute: ") + _info->_name);
    }
  }

  class TangoBackend : public DeviceBackendImpl {
   public:
    static boost::shared_ptr<DeviceBackend> createInstance(
        std::string address, std::map<std::string, std::string> parameters);

    explicit TangoBackend(std::string address);

    void open() override;
    void close() override;

    bool isFunctional() const override;

    RegisterCatalogue getRegisterCatalogue() const override;
    MetadataCatalogue getMetadataCatalogue() const override;

    std::string readDeviceInfo() override;
    void setException() override;

    void activateAsyncRead() noexcept override;

    template<typename UserType>
    boost::shared_ptr<NDRegisterAccessor<UserType>> getRegisterAccessor_impl(
        const RegisterPath& registerPathName, size_t numberOfWords, size_t wordOffsetInRegister, AccessModeFlags flags);

    DEFINE_VIRTUAL_FUNCTION_TEMPLATE_VTABLE_FILLER(TangoBackend, getRegisterAccessor_impl, 4);

   private:
    std::string _address{};
    std::shared_ptr<Tango::DeviceProxy> _deviceProxy;
    mutable bool _catalogueCompleted{false};
    mutable BackendRegisterCatalogue<TangoBackendRegisterInfo> _mutableCatalogue;
  };
} // namespace ChimeraTK