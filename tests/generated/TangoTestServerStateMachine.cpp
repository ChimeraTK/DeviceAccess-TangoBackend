/*----- PROTECTED REGION ID(TangoTestServerStateMachine.cpp) ENABLED START -----*/
/* clang-format on */
//=============================================================================
//
// file :        TangoTestServerStateMachine.cpp
//
// description : State machine file for the TangoTestServer class
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

#include "TangoTestServer.h"

/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TangoTestServer::TangoTestServerStateMachine.cpp

//================================================================
//  States  |  Description
//================================================================


namespace TangoTestServer_ns
{
//=================================================
//		Attributes Allowed Methods
//=================================================

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_DoubleScalar_allowed()
 * Description:  Execution allowed for DoubleScalar attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_DoubleScalar_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for DoubleScalar attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::DoubleScalarStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::DoubleScalarStateAllowed_WRITE

	//	Not any excluded states for DoubleScalar attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::DoubleScalarStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::DoubleScalarStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_DoubleRoScalar_allowed()
 * Description:  Execution allowed for DoubleRoScalar attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_DoubleRoScalar_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for DoubleRoScalar attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::DoubleRoScalarStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::DoubleRoScalarStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_DoubleWoScalar_allowed()
 * Description:  Execution allowed for DoubleWoScalar attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_DoubleWoScalar_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for DoubleWoScalar attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::DoubleWoScalarStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::DoubleWoScalarStateAllowed_WRITE

	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_FloatScalar_allowed()
 * Description:  Execution allowed for FloatScalar attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_FloatScalar_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for FloatScalar attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::FloatScalarStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::FloatScalarStateAllowed_WRITE

	//	Not any excluded states for FloatScalar attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::FloatScalarStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::FloatScalarStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_FloatRoScalar_allowed()
 * Description:  Execution allowed for FloatRoScalar attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_FloatRoScalar_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for FloatRoScalar attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::FloatRoScalarStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::FloatRoScalarStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_FloatWoScalar_allowed()
 * Description:  Execution allowed for FloatWoScalar attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_FloatWoScalar_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for FloatWoScalar attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::FloatWoScalarStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::FloatWoScalarStateAllowed_WRITE

	//	Not any excluded states for FloatWoScalar attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::FloatWoScalarStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::FloatWoScalarStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_IntScalar_allowed()
 * Description:  Execution allowed for IntScalar attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_IntScalar_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for IntScalar attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::IntScalarStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::IntScalarStateAllowed_WRITE

	//	Not any excluded states for IntScalar attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::IntScalarStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::IntScalarStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_IntRoScalar_allowed()
 * Description:  Execution allowed for IntRoScalar attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_IntRoScalar_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for IntRoScalar attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::IntRoScalarStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::IntRoScalarStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_IntWoScalar_allowed()
 * Description:  Execution allowed for IntWoScalar attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_IntWoScalar_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for IntWoScalar attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::IntWoScalarStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::IntWoScalarStateAllowed_WRITE

	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_StringScalar_allowed()
 * Description:  Execution allowed for StringScalar attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_StringScalar_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for StringScalar attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::StringScalarStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::StringScalarStateAllowed_WRITE

	//	Not any excluded states for StringScalar attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::StringScalarStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::StringScalarStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_StringRoScalar_allowed()
 * Description:  Execution allowed for StringRoScalar attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_StringRoScalar_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for StringRoScalar attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::StringRoScalarStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::StringRoScalarStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_StringWoScalar_allowed()
 * Description:  Execution allowed for StringWoScalar attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_StringWoScalar_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for StringWoScalar attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::StringWoScalarStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::StringWoScalarStateAllowed_WRITE

	//	Not any excluded states for StringWoScalar attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::StringWoScalarStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::StringWoScalarStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_BooleanScalar_allowed()
 * Description:  Execution allowed for BooleanScalar attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_BooleanScalar_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for BooleanScalar attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::BooleanScalarStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::BooleanScalarStateAllowed_WRITE

	//	Not any excluded states for BooleanScalar attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::BooleanScalarStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::BooleanScalarStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_BooleanRoScalar_allowed()
 * Description:  Execution allowed for BooleanRoScalar attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_BooleanRoScalar_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for BooleanRoScalar attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::BooleanRoScalarStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::BooleanRoScalarStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_BooleanWoScalar_allowed()
 * Description:  Execution allowed for BooleanWoScalar attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_BooleanWoScalar_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for BooleanWoScalar attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::BooleanWoScalarStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::BooleanWoScalarStateAllowed_WRITE

	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_DoubleSpectrum_allowed()
 * Description:  Execution allowed for DoubleSpectrum attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_DoubleSpectrum_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for DoubleSpectrum attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::DoubleSpectrumStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::DoubleSpectrumStateAllowed_WRITE

	//	Not any excluded states for DoubleSpectrum attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::DoubleSpectrumStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::DoubleSpectrumStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_DoubleRoSpectrum_allowed()
 * Description:  Execution allowed for DoubleRoSpectrum attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_DoubleRoSpectrum_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for DoubleRoSpectrum attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::DoubleRoSpectrumStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::DoubleRoSpectrumStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_DoubleWoSpectrum_allowed()
 * Description:  Execution allowed for DoubleWoSpectrum attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_DoubleWoSpectrum_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for DoubleWoSpectrum attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::DoubleWoSpectrumStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::DoubleWoSpectrumStateAllowed_WRITE

	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_FloatSpectrum_allowed()
 * Description:  Execution allowed for FloatSpectrum attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_FloatSpectrum_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for FloatSpectrum attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::FloatSpectrumStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::FloatSpectrumStateAllowed_WRITE

	//	Not any excluded states for FloatSpectrum attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::FloatSpectrumStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::FloatSpectrumStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_FloatRoSpectrum_allowed()
 * Description:  Execution allowed for FloatRoSpectrum attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_FloatRoSpectrum_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for FloatRoSpectrum attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::FloatRoSpectrumStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::FloatRoSpectrumStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_FloatWoSpectrum_allowed()
 * Description:  Execution allowed for FloatWoSpectrum attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_FloatWoSpectrum_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for FloatWoSpectrum attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::FloatWoSpectrumStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::FloatWoSpectrumStateAllowed_WRITE

	//	Not any excluded states for FloatWoSpectrum attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::FloatWoSpectrumStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::FloatWoSpectrumStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_IntSpectrum_allowed()
 * Description:  Execution allowed for IntSpectrum attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_IntSpectrum_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for IntSpectrum attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::IntSpectrumStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::IntSpectrumStateAllowed_WRITE

	//	Not any excluded states for IntSpectrum attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::IntSpectrumStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::IntSpectrumStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_IntRoSpectrum_allowed()
 * Description:  Execution allowed for IntRoSpectrum attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_IntRoSpectrum_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for IntRoSpectrum attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::IntRoSpectrumStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::IntRoSpectrumStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_IntWoSpectrum_allowed()
 * Description:  Execution allowed for IntWoSpectrum attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_IntWoSpectrum_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for IntWoSpectrum attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::IntWoSpectrumStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::IntWoSpectrumStateAllowed_WRITE

	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_StringSpectrum_allowed()
 * Description:  Execution allowed for StringSpectrum attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_StringSpectrum_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for StringSpectrum attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::StringSpectrumStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::StringSpectrumStateAllowed_WRITE

	//	Not any excluded states for StringSpectrum attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::StringSpectrumStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::StringSpectrumStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_StringRoSpectrum_allowed()
 * Description:  Execution allowed for StringRoSpectrum attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_StringRoSpectrum_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for StringRoSpectrum attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::StringRoSpectrumStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::StringRoSpectrumStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : TangoTestServer::is_StringWoSpectrum_allowed()
 * Description:  Execution allowed for StringWoSpectrum attribute
 */
//--------------------------------------------------------
bool TangoTestServer::is_StringWoSpectrum_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for StringWoSpectrum attribute in Write access.
	/*----- PROTECTED REGION ID(TangoTestServer::StringWoSpectrumStateAllowed_WRITE) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::StringWoSpectrumStateAllowed_WRITE

	//	Not any excluded states for StringWoSpectrum attribute in read access.
	/*----- PROTECTED REGION ID(TangoTestServer::StringWoSpectrumStateAllowed_READ) ENABLED START -----*/
	/* clang-format on */
	/* clang-format off */
	/*----- PROTECTED REGION END -----*/	//	TangoTestServer::StringWoSpectrumStateAllowed_READ
	return true;
}


//=================================================
//		Commands Allowed Methods
//=================================================


/*----- PROTECTED REGION ID(TangoTestServer::TangoTestServerStateAllowed.AdditionalMethods) ENABLED START -----*/
/* clang-format on */
//	Additional Methods
/* clang-format off */
/*----- PROTECTED REGION END -----*/	//	TangoTestServer::TangoTestServerStateAllowed.AdditionalMethods

}	//	End of namespace
