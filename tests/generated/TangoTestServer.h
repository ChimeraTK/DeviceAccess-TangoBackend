/*----- PROTECTED REGION ID(TangoTestServer.h) ENABLED START -----*/
/* clang-format on */
//=============================================================================
//
// file :        TangoTestServer.h
//
// description : Include file for the TangoTestServer class
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


#ifndef TangoTestServer_H
#define TangoTestServer_H

#include <tango/tango.h>

#include <atomic>

/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TangoTestServer.h

#ifdef TANGO_LOG
	// cppTango after c934adea (Merge branch 'remove-cout-definition' into 'main', 2022-05-23)
	// nothing to do
#else
	// cppTango 9.3-backports and older
	#define TANGO_LOG       cout
	#define TANGO_LOG_INFO  cout2
	#define TANGO_LOG_DEBUG cout3
#endif // TANGO_LOG

/**
 *  TangoTestServer class description:
 *
 */


namespace TangoTestServer_ns
{
/*----- PROTECTED REGION ID(TangoTestServer::Additional Class Declarations) ENABLED START -----*/
/* clang-format on */
//	Additional Class Declarations
/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TangoTestServer::Additional Class Declarations

class TangoTestServer : public TANGO_BASE_CLASS
{

/*----- PROTECTED REGION ID(TangoTestServer::Data Members) ENABLED START -----*/
/* clang-format on */
public:
 std::atomic_bool runtimeError{false};
 /* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TangoTestServer::Data Members


//	Attribute data members
public:
	Tango::DevDouble	*attr_DoubleScalar_read;
        Tango::DevFloat	*attr_FloatScalar_read;
	Tango::DevLong	*attr_IntScalar_read;
        Tango::DevString	*attr_StringScalar_read;
	Tango::DevBoolean	*attr_BooleanScalar_read;
        Tango::DevDouble	*attr_DoubleSpectrum_read;
        Tango::DevFloat	*attr_FloatSpectrum_read;
	Tango::DevLong	*attr_IntSpectrum_read;
        Tango::DevString	*attr_StringSpectrum_read;

//	Constructors and destructors
public:
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	TangoTestServer(Tango::DeviceClass *cl,std::string &s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	TangoTestServer(Tango::DeviceClass *cl,const char *s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device name
	 *	@param d	Device description.
	 */
	TangoTestServer(Tango::DeviceClass *cl,const char *s,const char *d);
	/**
	 * The device object destructor.
	 */
	~TangoTestServer();


//	Miscellaneous methods
public:
	/*
	 *	will be called at device destruction or at init command.
	 */
	void delete_device();
	/*
	 *	Initialize the device
	 */
	virtual void init_device();
	/*
	 *	Always executed method before execution command method.
	 */
	virtual void always_executed_hook();


//	Attribute methods
public:
	//--------------------------------------------------------
	/*
	 *	Method      : TangoTestServer::read_attr_hardware()
	 * Description:  Hardware acquisition for attributes.
	 */
	//--------------------------------------------------------
	virtual void read_attr_hardware(std::vector<long> &attr_list);
	//--------------------------------------------------------
	/*
	 *	Method      : TangoTestServer::write_attr_hardware()
	 * Description:  Hardware writing for attributes.
	 */
	//--------------------------------------------------------
	virtual void write_attr_hardware(std::vector<long> &attr_list);

/**
 *	Attribute DoubleScalar related methods
 *
 *
 *	Data type:  Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_DoubleScalar(Tango::Attribute &attr);
	virtual void write_DoubleScalar(Tango::WAttribute &attr);
	virtual bool is_DoubleScalar_allowed(Tango::AttReqType type);
/**
 *	Attribute DoubleRoScalar related methods
 *
 *
 *	Data type:  Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_DoubleRoScalar(Tango::Attribute &attr);
	virtual bool is_DoubleRoScalar_allowed(Tango::AttReqType type);
/**
 *	Attribute DoubleWoScalar related methods
 *
 *
 *	Data type:  Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void write_DoubleWoScalar(Tango::WAttribute &attr);
	virtual bool is_DoubleWoScalar_allowed(Tango::AttReqType type);
/**
 *	Attribute FloatScalar related methods
 *
 *
 *	Data type:  Tango::DevFloat
 *	Attr type:	Scalar
 */
	virtual void read_FloatScalar(Tango::Attribute &attr);
	virtual void write_FloatScalar(Tango::WAttribute &attr);
	virtual bool is_FloatScalar_allowed(Tango::AttReqType type);
/**
 *	Attribute FloatRoScalar related methods
 *
 *
 *	Data type:  Tango::DevFloat
 *	Attr type:	Scalar
 */
	virtual void read_FloatRoScalar(Tango::Attribute &attr);
	virtual bool is_FloatRoScalar_allowed(Tango::AttReqType type);
/**
 *	Attribute FloatWoScalar related methods
 *
 *
 *	Data type:  Tango::DevFloat
 *	Attr type:	Scalar
 */
	virtual void read_FloatWoScalar(Tango::Attribute &attr);
	virtual void write_FloatWoScalar(Tango::WAttribute &attr);
	virtual bool is_FloatWoScalar_allowed(Tango::AttReqType type);
/**
 *	Attribute IntScalar related methods
 *
 *
 *	Data type:  Tango::DevLong
 *	Attr type:	Scalar
 */
	virtual void read_IntScalar(Tango::Attribute &attr);
	virtual void write_IntScalar(Tango::WAttribute &attr);
	virtual bool is_IntScalar_allowed(Tango::AttReqType type);
/**
 *	Attribute IntRoScalar related methods
 *
 *
 *	Data type:  Tango::DevLong
 *	Attr type:	Scalar
 */
	virtual void read_IntRoScalar(Tango::Attribute &attr);
	virtual bool is_IntRoScalar_allowed(Tango::AttReqType type);
/**
 *	Attribute IntWoScalar related methods
 *
 *
 *	Data type:  Tango::DevLong
 *	Attr type:	Scalar
 */
	virtual void write_IntWoScalar(Tango::WAttribute &attr);
	virtual bool is_IntWoScalar_allowed(Tango::AttReqType type);
/**
 *	Attribute StringScalar related methods
 *
 *
 *	Data type:  Tango::DevString
 *	Attr type:	Scalar
 */
	virtual void read_StringScalar(Tango::Attribute &attr);
	virtual void write_StringScalar(Tango::WAttribute &attr);
	virtual bool is_StringScalar_allowed(Tango::AttReqType type);
/**
 *	Attribute StringRoScalar related methods
 *
 *
 *	Data type:  Tango::DevString
 *	Attr type:	Scalar
 */
	virtual void read_StringRoScalar(Tango::Attribute &attr);
	virtual bool is_StringRoScalar_allowed(Tango::AttReqType type);
/**
 *	Attribute StringWoScalar related methods
 *
 *
 *	Data type:  Tango::DevString
 *	Attr type:	Scalar
 */
	virtual void write_StringWoScalar(Tango::WAttribute &attr);
	virtual bool is_StringWoScalar_allowed(Tango::AttReqType type);
/**
 *	Attribute BooleanScalar related methods
 *
 *
 *	Data type:  Tango::DevBoolean
 *	Attr type:	Scalar
 */
	virtual void read_BooleanScalar(Tango::Attribute &attr);
	virtual void write_BooleanScalar(Tango::WAttribute &attr);
	virtual bool is_BooleanScalar_allowed(Tango::AttReqType type);
/**
 *	Attribute BooleanRoScalar related methods
 *
 *
 *	Data type:  Tango::DevBoolean
 *	Attr type:	Scalar
 */
	virtual void read_BooleanRoScalar(Tango::Attribute &attr);
	virtual bool is_BooleanRoScalar_allowed(Tango::AttReqType type);
/**
 *	Attribute BooleanWoScalar related methods
 *
 *
 *	Data type:  Tango::DevBoolean
 *	Attr type:	Scalar
 */
	virtual void write_BooleanWoScalar(Tango::WAttribute &attr);
	virtual bool is_BooleanWoScalar_allowed(Tango::AttReqType type);
/**
 *	Attribute DoubleSpectrum related methods
 *
 *
 *	Data type:  Tango::DevDouble
 *	Attr type:	Spectrum max = 10
 */
	virtual void read_DoubleSpectrum(Tango::Attribute &attr);
	virtual void write_DoubleSpectrum(Tango::WAttribute &attr);
	virtual bool is_DoubleSpectrum_allowed(Tango::AttReqType type);
/**
 *	Attribute DoubleRoSpectrum related methods
 *
 *
 *	Data type:  Tango::DevDouble
 *	Attr type:	Spectrum max = 10
 */
	virtual void read_DoubleRoSpectrum(Tango::Attribute &attr);
	virtual bool is_DoubleRoSpectrum_allowed(Tango::AttReqType type);
/**
 *	Attribute DoubleWoSpectrum related methods
 *
 *
 *	Data type:  Tango::DevDouble
 *	Attr type:	Spectrum max = 10
 */
	virtual void write_DoubleWoSpectrum(Tango::WAttribute &attr);
	virtual bool is_DoubleWoSpectrum_allowed(Tango::AttReqType type);
/**
 *	Attribute FloatSpectrum related methods
 *
 *
 *	Data type:  Tango::DevFloat
 *	Attr type:	Spectrum max = 10
 */
	virtual void read_FloatSpectrum(Tango::Attribute &attr);
	virtual void write_FloatSpectrum(Tango::WAttribute &attr);
	virtual bool is_FloatSpectrum_allowed(Tango::AttReqType type);
/**
 *	Attribute FloatRoSpectrum related methods
 *
 *
 *	Data type:  Tango::DevFloat
 *	Attr type:	Spectrum max = 10
 */
	virtual void read_FloatRoSpectrum(Tango::Attribute &attr);
	virtual bool is_FloatRoSpectrum_allowed(Tango::AttReqType type);
/**
 *	Attribute FloatWoSpectrum related methods
 *
 *
 *	Data type:  Tango::DevFloat
 *	Attr type:	Spectrum max = 10
 */
	virtual void read_FloatWoSpectrum(Tango::Attribute &attr);
	virtual void write_FloatWoSpectrum(Tango::WAttribute &attr);
	virtual bool is_FloatWoSpectrum_allowed(Tango::AttReqType type);
/**
 *	Attribute IntSpectrum related methods
 *
 *
 *	Data type:  Tango::DevLong
 *	Attr type:	Spectrum max = 10
 */
	virtual void read_IntSpectrum(Tango::Attribute &attr);
	virtual void write_IntSpectrum(Tango::WAttribute &attr);
	virtual bool is_IntSpectrum_allowed(Tango::AttReqType type);
/**
 *	Attribute IntRoSpectrum related methods
 *
 *
 *	Data type:  Tango::DevLong
 *	Attr type:	Spectrum max = 10
 */
	virtual void read_IntRoSpectrum(Tango::Attribute &attr);
	virtual bool is_IntRoSpectrum_allowed(Tango::AttReqType type);
/**
 *	Attribute IntWoSpectrum related methods
 *
 *
 *	Data type:  Tango::DevLong
 *	Attr type:	Spectrum max = 10
 */
	virtual void write_IntWoSpectrum(Tango::WAttribute &attr);
	virtual bool is_IntWoSpectrum_allowed(Tango::AttReqType type);
/**
 *	Attribute StringSpectrum related methods
 *
 *
 *	Data type:  Tango::DevString
 *	Attr type:	Spectrum max = 10
 */
	virtual void read_StringSpectrum(Tango::Attribute &attr);
	virtual void write_StringSpectrum(Tango::WAttribute &attr);
	virtual bool is_StringSpectrum_allowed(Tango::AttReqType type);
/**
 *	Attribute StringRoSpectrum related methods
 *
 *
 *	Data type:  Tango::DevString
 *	Attr type:	Spectrum max = 10
 */
	virtual void read_StringRoSpectrum(Tango::Attribute &attr);
	virtual bool is_StringRoSpectrum_allowed(Tango::AttReqType type);
/**
 *	Attribute StringWoSpectrum related methods
 *
 *
 *	Data type:  Tango::DevString
 *	Attr type:	Spectrum max = 10
 */
	virtual void read_StringWoSpectrum(Tango::Attribute &attr);
	virtual void write_StringWoSpectrum(Tango::WAttribute &attr);
	virtual bool is_StringWoSpectrum_allowed(Tango::AttReqType type);


	//--------------------------------------------------------
	/**
	 *	Method      : TangoTestServer::add_dynamic_attributes()
	 * Description:  Add dynamic attributes if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_attributes();




//	Command related methods
public:


	//--------------------------------------------------------
	/**
	 *	Method      : TangoTestServer::add_dynamic_commands()
	 * Description:  Add dynamic commands if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_commands();

/*----- PROTECTED REGION ID(TangoTestServer::Additional Method prototypes) ENABLED START -----*/
/* clang-format on */
//	Additional Method prototypes
/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TangoTestServer::Additional Method prototypes
};

/*----- PROTECTED REGION ID(TangoTestServer::Additional Classes Definitions) ENABLED START -----*/
/* clang-format on */
//	Additional Classes Definitions
/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TangoTestServer::Additional Classes Definitions

}	//	End of namespace

#endif   //	TangoTestServer_H
