/*----- PROTECTED REGION ID(TangoTestServerClass.h) ENABLED START -----*/
/* clang-format on */
//=============================================================================
//
// file :        TangoTestServerClass.h
//
// description : Include for the TangoTestServer root class.
//               This class is the singleton class for
//                the TangoTestServer device class.
//               It contains all properties and methods which the
//               TangoTestServer requires only once e.g. the commands.
//
// project :     TangoTestServer
//
// This file is part of Tango device class.
//
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
//
//
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================


#ifndef TangoTestServerClass_H
#define TangoTestServerClass_H

#include "TangoTestServer.h"
#include <tango/tango.h>

#include <atomic>

/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TangoTestServerClass.h


namespace TangoTestServer_ns
{
/*----- PROTECTED REGION ID(TangoTestServerClass::classes for dynamic creation) ENABLED START -----*/
/* clang-format on */

class ExceptionAttribute : public Tango::Attr {
 public:
  using Tango ::Attr::Attr;

  void read(Tango::DeviceImpl* dev, [[maybe_unused]] Tango::Attribute& att) override {
    if((static_cast<TangoTestServer*>(dev))->runtimeError.load()) {
      Tango::Except::throw_exception("Exception test", "Requested exception", "Yes, really");
    }
  }

  void write(Tango::DeviceImpl* dev, [[maybe_unused]] Tango::WAttribute& att) override {
    if((static_cast<TangoTestServer*>(dev))->runtimeError.load()) {
      Tango::Except::throw_exception("Exception test", "Requested exception", "Yes, really");
    }
  }
};

class ExceptionSpectrumAttribute : public Tango::SpectrumAttr {
 public:
  using Tango::SpectrumAttr::SpectrumAttr;

  void read(Tango::DeviceImpl* dev, [[maybe_unused]] Tango::Attribute& att) override {
    if((static_cast<TangoTestServer*>(dev))->runtimeError.load()) {
      Tango::Except::throw_exception("Exception test", "Requested exception", "Yes, really");
    }
  }

  void write(Tango::DeviceImpl* dev, [[maybe_unused]] Tango::WAttribute& att) override {
    if((static_cast<TangoTestServer*>(dev))->runtimeError.load()) {
      Tango::Except::throw_exception("Exception test", "Requested exception", "Yes, really");
    }
  }
};

/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TangoTestServerClass::classes for dynamic creation

//=========================================
//	Define classes for attributes
//=========================================
//	Attribute DoubleScalar class definition
class DoubleScalarAttrib: public ExceptionAttribute
{
public:
	DoubleScalarAttrib():ExceptionAttribute("DoubleScalar",
			Tango::DEV_DOUBLE, Tango::READ_WRITE) {};
	~DoubleScalarAttrib() = default;
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_DoubleScalar(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_DoubleScalar(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_DoubleScalar_allowed(ty);}
};

//	Attribute DoubleRoScalar class definition
class DoubleRoScalarAttrib: public ExceptionAttribute
{
public:
	DoubleRoScalarAttrib():ExceptionAttribute("DoubleRoScalar",
			Tango::DEV_DOUBLE, Tango::READ) {};
	~DoubleRoScalarAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_DoubleRoScalar(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_DoubleRoScalar_allowed(ty);}
};

//	Attribute DoubleWoScalar class definition
class DoubleWoScalarAttrib: public ExceptionAttribute
{
public:
	DoubleWoScalarAttrib():ExceptionAttribute("DoubleWoScalar",
			Tango::DEV_DOUBLE, Tango::WRITE) {};
	~DoubleWoScalarAttrib() {};
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_DoubleWoScalar(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_DoubleWoScalar_allowed(ty);}
};

//	Attribute FloatScalar class definition
class FloatScalarAttrib: public ExceptionAttribute
{
public:
	FloatScalarAttrib():ExceptionAttribute("FloatScalar",
			Tango::DEV_FLOAT, Tango::READ_WRITE) {};
	~FloatScalarAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_FloatScalar(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_FloatScalar(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_FloatScalar_allowed(ty);}
};

//	Attribute FloatRoScalar class definition
class FloatRoScalarAttrib: public ExceptionAttribute
{
public:
	FloatRoScalarAttrib():ExceptionAttribute("FloatRoScalar",
			Tango::DEV_FLOAT, Tango::READ) {};
	~FloatRoScalarAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_FloatRoScalar(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_FloatRoScalar_allowed(ty);}
};

//	Attribute FloatWoScalar class definition
class FloatWoScalarAttrib: public ExceptionAttribute
{
public:
	FloatWoScalarAttrib():ExceptionAttribute("FloatWoScalar",
			Tango::DEV_FLOAT, Tango::READ_WRITE) {};
	~FloatWoScalarAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_FloatWoScalar(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_FloatWoScalar(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_FloatWoScalar_allowed(ty);}
};

//	Attribute IntScalar class definition
class IntScalarAttrib: public ExceptionAttribute
{
public:
	IntScalarAttrib():ExceptionAttribute("IntScalar",
			Tango::DEV_LONG, Tango::READ_WRITE) {};
	~IntScalarAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_IntScalar(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_IntScalar(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_IntScalar_allowed(ty);}
};

//	Attribute IntRoScalar class definition
class IntRoScalarAttrib: public ExceptionAttribute
{
public:
	IntRoScalarAttrib():ExceptionAttribute("IntRoScalar",
			Tango::DEV_LONG, Tango::READ) {};
	~IntRoScalarAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_IntRoScalar(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_IntRoScalar_allowed(ty);}
};

//	Attribute IntWoScalar class definition
class IntWoScalarAttrib: public ExceptionAttribute
{
public:
	IntWoScalarAttrib():ExceptionAttribute("IntWoScalar",
			Tango::DEV_LONG, Tango::WRITE) {};
	~IntWoScalarAttrib() {};
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_IntWoScalar(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_IntWoScalar_allowed(ty);}
};

//	Attribute StringScalar class definition
class StringScalarAttrib: public ExceptionAttribute
{
public:
	StringScalarAttrib():ExceptionAttribute("StringScalar",
			Tango::DEV_STRING, Tango::READ_WRITE) {};
	~StringScalarAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_StringScalar(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_StringScalar(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_StringScalar_allowed(ty);}
};

//	Attribute StringRoScalar class definition
class StringRoScalarAttrib: public ExceptionAttribute
{
public:
	StringRoScalarAttrib():ExceptionAttribute("StringRoScalar",
			Tango::DEV_STRING, Tango::READ) {};
	~StringRoScalarAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_StringRoScalar(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_StringRoScalar_allowed(ty);}
};

//	Attribute StringWoScalar class definition
class StringWoScalarAttrib: public ExceptionAttribute
{
public:
	StringWoScalarAttrib():ExceptionAttribute("StringWoScalar",
			Tango::DEV_STRING, Tango::READ_WRITE) {};
	~StringWoScalarAttrib() {};
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_StringWoScalar(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_StringWoScalar_allowed(ty);}
};

//	Attribute BooleanScalar class definition
class BooleanScalarAttrib: public ExceptionAttribute
{
public:
	BooleanScalarAttrib():ExceptionAttribute("BooleanScalar",
			Tango::DEV_BOOLEAN, Tango::READ_WRITE) {};
	~BooleanScalarAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_BooleanScalar(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_BooleanScalar(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_BooleanScalar_allowed(ty);}
};

//	Attribute BooleanRoScalar class definition
class BooleanRoScalarAttrib: public ExceptionAttribute
{
public:
	BooleanRoScalarAttrib():ExceptionAttribute("BooleanRoScalar",
			Tango::DEV_BOOLEAN, Tango::READ) {};
	~BooleanRoScalarAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_BooleanRoScalar(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_BooleanRoScalar_allowed(ty);}
};

//	Attribute BooleanWoScalar class definition
class BooleanWoScalarAttrib: public ExceptionAttribute
{
public:
	BooleanWoScalarAttrib():ExceptionAttribute("BooleanWoScalar",
			Tango::DEV_BOOLEAN, Tango::WRITE) {};
	~BooleanWoScalarAttrib() {};
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_BooleanWoScalar(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_BooleanWoScalar_allowed(ty);}
};

//	Attribute DoubleSpectrum class definition
class DoubleSpectrumAttrib: public ExceptionSpectrumAttribute
{
public:
	// Constants for DoubleSpectrum attribute
	constexpr static long X_DATA_SIZE = 10;
	DoubleSpectrumAttrib():ExceptionSpectrumAttribute("DoubleSpectrum",
			Tango::DEV_DOUBLE, Tango::READ_WRITE, DoubleSpectrumAttrib::X_DATA_SIZE) {};
	~DoubleSpectrumAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionSpectrumAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_DoubleSpectrum(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionSpectrumAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_DoubleSpectrum(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_DoubleSpectrum_allowed(ty);}
};

//	Attribute DoubleRoSpectrum class definition
class DoubleRoSpectrumAttrib: public ExceptionSpectrumAttribute
{
public:
	// Constants for DoubleRoSpectrum attribute
	constexpr static long X_DATA_SIZE = 10;
	DoubleRoSpectrumAttrib():ExceptionSpectrumAttribute("DoubleRoSpectrum",
			Tango::DEV_DOUBLE, Tango::READ, DoubleRoSpectrumAttrib::X_DATA_SIZE) {};
	~DoubleRoSpectrumAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionSpectrumAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_DoubleRoSpectrum(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_DoubleRoSpectrum_allowed(ty);}
};

//	Attribute DoubleWoSpectrum class definition
class DoubleWoSpectrumAttrib: public ExceptionSpectrumAttribute
{
public:
	// Constants for DoubleWoSpectrum attribute
	constexpr static long X_DATA_SIZE = 10;
	DoubleWoSpectrumAttrib():ExceptionSpectrumAttribute("DoubleWoSpectrum",
			Tango::DEV_DOUBLE, Tango::WRITE, DoubleWoSpectrumAttrib::X_DATA_SIZE) {};
	~DoubleWoSpectrumAttrib() {};
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionSpectrumAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_DoubleWoSpectrum(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_DoubleWoSpectrum_allowed(ty);}
};

//	Attribute FloatSpectrum class definition
class FloatSpectrumAttrib: public ExceptionSpectrumAttribute
{
public:
	// Constants for FloatSpectrum attribute
	constexpr static long X_DATA_SIZE = 10;
	FloatSpectrumAttrib():ExceptionSpectrumAttribute("FloatSpectrum",
			Tango::DEV_FLOAT, Tango::READ_WRITE, FloatSpectrumAttrib::X_DATA_SIZE) {};
	~FloatSpectrumAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionSpectrumAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_FloatSpectrum(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionSpectrumAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_FloatSpectrum(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_FloatSpectrum_allowed(ty);}
};

//	Attribute FloatRoSpectrum class definition
class FloatRoSpectrumAttrib: public ExceptionSpectrumAttribute
{
public:
	// Constants for FloatRoSpectrum attribute
	constexpr static long X_DATA_SIZE = 10;
	FloatRoSpectrumAttrib():ExceptionSpectrumAttribute("FloatRoSpectrum",
			Tango::DEV_FLOAT, Tango::READ, FloatRoSpectrumAttrib::X_DATA_SIZE) {};
	~FloatRoSpectrumAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionSpectrumAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_FloatRoSpectrum(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_FloatRoSpectrum_allowed(ty);}
};

//	Attribute FloatWoSpectrum class definition
class FloatWoSpectrumAttrib: public ExceptionSpectrumAttribute
{
public:
	// Constants for FloatWoSpectrum attribute
	constexpr static long X_DATA_SIZE = 10;
	FloatWoSpectrumAttrib():ExceptionSpectrumAttribute("FloatWoSpectrum",
			Tango::DEV_FLOAT, Tango::READ_WRITE, FloatWoSpectrumAttrib::X_DATA_SIZE) {};
	~FloatWoSpectrumAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionSpectrumAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_FloatWoSpectrum(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionSpectrumAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_FloatWoSpectrum(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_FloatWoSpectrum_allowed(ty);}
};

//	Attribute IntSpectrum class definition
class IntSpectrumAttrib: public ExceptionSpectrumAttribute
{
public:
	// Constants for IntSpectrum attribute
	constexpr static long X_DATA_SIZE = 10;
	IntSpectrumAttrib():ExceptionSpectrumAttribute("IntSpectrum",
			Tango::DEV_LONG, Tango::READ_WRITE, IntSpectrumAttrib::X_DATA_SIZE) {};
	~IntSpectrumAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionSpectrumAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_IntSpectrum(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionSpectrumAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_IntSpectrum(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_IntSpectrum_allowed(ty);}
};

//	Attribute IntRoSpectrum class definition
class IntRoSpectrumAttrib: public ExceptionSpectrumAttribute
{
public:
	// Constants for IntRoSpectrum attribute
	constexpr static long X_DATA_SIZE = 10;
	IntRoSpectrumAttrib():ExceptionSpectrumAttribute("IntRoSpectrum",
			Tango::DEV_LONG, Tango::READ, IntRoSpectrumAttrib::X_DATA_SIZE) {};
	~IntRoSpectrumAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionSpectrumAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_IntRoSpectrum(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_IntRoSpectrum_allowed(ty);}
};

//	Attribute IntWoSpectrum class definition
class IntWoSpectrumAttrib: public ExceptionSpectrumAttribute
{
public:
	// Constants for IntWoSpectrum attribute
	constexpr static long X_DATA_SIZE = 10;
	IntWoSpectrumAttrib():ExceptionSpectrumAttribute("IntWoSpectrum",
			Tango::DEV_LONG, Tango::WRITE, IntWoSpectrumAttrib::X_DATA_SIZE) {};
	~IntWoSpectrumAttrib() {};
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionSpectrumAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_IntWoSpectrum(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_IntWoSpectrum_allowed(ty);}
};

//	Attribute StringSpectrum class definition
class StringSpectrumAttrib: public ExceptionSpectrumAttribute
{
public:
	// Constants for StringSpectrum attribute
	constexpr static long X_DATA_SIZE = 10;
	StringSpectrumAttrib():ExceptionSpectrumAttribute("StringSpectrum",
			Tango::DEV_STRING, Tango::READ_WRITE, StringSpectrumAttrib::X_DATA_SIZE) {};
	~StringSpectrumAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionSpectrumAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_StringSpectrum(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionSpectrumAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_StringSpectrum(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_StringSpectrum_allowed(ty);}
};

//	Attribute StringRoSpectrum class definition
class StringRoSpectrumAttrib: public ExceptionSpectrumAttribute
{
public:
	// Constants for StringRoSpectrum attribute
	constexpr static long X_DATA_SIZE = 10;
	StringRoSpectrumAttrib():ExceptionSpectrumAttribute("StringRoSpectrum",
			Tango::DEV_STRING, Tango::READ, StringRoSpectrumAttrib::X_DATA_SIZE) {};
	~StringRoSpectrumAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionSpectrumAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_StringRoSpectrum(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_StringRoSpectrum_allowed(ty);}
};

//	Attribute StringWoSpectrum class definition
class StringWoSpectrumAttrib: public ExceptionSpectrumAttribute
{
public:
	// Constants for StringWoSpectrum attribute
	constexpr static long X_DATA_SIZE = 10;
	StringWoSpectrumAttrib():ExceptionSpectrumAttribute("StringWoSpectrum",
			Tango::DEV_STRING, Tango::READ_WRITE, StringWoSpectrumAttrib::X_DATA_SIZE) {};
	~StringWoSpectrumAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionSpectrumAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_StringWoSpectrum(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionSpectrumAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_StringWoSpectrum(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_StringWoSpectrum_allowed(ty);}
};

//	Attribute StringSpectrum class definition
class BooleanSpectrumAttrib: public ExceptionSpectrumAttribute
{
public:
	// Constants for StringSpectrum attribute
	constexpr static long X_DATA_SIZE = 10;
	BooleanSpectrumAttrib():ExceptionSpectrumAttribute("BooleanSpectrum",
			Tango::DEV_BOOLEAN, Tango::READ_WRITE, BooleanSpectrumAttrib::X_DATA_SIZE) {};
	~BooleanSpectrumAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{ExceptionSpectrumAttribute::read(dev, att); (static_cast<TangoTestServer *>(dev))->read_BooleanSpectrum(att);}
	virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att)
		{ExceptionSpectrumAttribute::write(dev, att); (static_cast<TangoTestServer *>(dev))->write_BooleanSpectrum(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoTestServer *>(dev))->is_BooleanSpectrum_allowed(ty);}
};


/**
 *	The TangoTestServerClass singleton definition
 */

#ifdef _TG_WINDOWS_
class __declspec(dllexport)  TangoTestServerClass : public Tango::DeviceClass
#else
class TangoTestServerClass : public Tango::DeviceClass
#endif
{
	/*----- PROTECTED REGION ID(TangoTestServerClass::Additional DServer data members) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServerClass::Additional DServer data members

	public:
		//	write class properties data members
		Tango::DbData	cl_prop;
		Tango::DbData	cl_def_prop;
		Tango::DbData	dev_def_prop;
		//	Method prototypes
		static TangoTestServerClass *init(const char *);
		static TangoTestServerClass *instance();
		~TangoTestServerClass();
		Tango::DbDatum	get_class_property(std::string &);
		Tango::DbDatum	get_default_device_property(std::string &);
		Tango::DbDatum	get_default_class_property(std::string &);

	protected:
		TangoTestServerClass(std::string &);
		static TangoTestServerClass *_instance;
		void command_factory();
		void attribute_factory(std::vector<Tango::Attr *> &);
		void pipe_factory();
		void write_class_property();
		void set_default_property();
		void get_class_property();
		std::string get_cvstag();
		std::string get_cvsroot();

	private:
		void device_factory(TANGO_UNUSED(const Tango::DevVarStringArray *));
		void create_static_attribute_list(std::vector<Tango::Attr *> &);
		void erase_dynamic_attributes(const Tango::DevVarStringArray *,std::vector<Tango::Attr *> &);
		std::vector<std::string>	defaultAttList;
		Tango::Attr *get_attr_object_by_name(std::vector<Tango::Attr *> &att_list, std::string attname);
};

}	//	End of namespace

#endif   //	TangoTestServer_H
