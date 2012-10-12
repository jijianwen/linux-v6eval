// SCTP Conformance Test Suite Implementation
// (C) Copyright Fujitsu Ltd. 2008, 2009
//
// This file is part of the SCTP Conformance Test Suite implementation.
//
// The SCTP Conformance Test Suite implementation is free software;
// you can redistribute it and/or modify it under the terms of
// the GNU General Public License version 2 as published by
// the Free Software Foundation.
//
// The SCTP Conformance Test Suite implementation is distributed in the
// hope that it will be useful, but WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU CC; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
//
// Please send any bug reports or fixes you make to the
// email address(es):
//    networktest sctp <networktest-sctp@lists.sourceforge.net>
//
// Or submit a bug report through the following website:
//    http://networktest.sourceforge.net/
//
// Written or modified by:
//    Hiroaki Kago <linuxsctp-kg@ml.css.fujitsu.com>
//    Wei Yongjun <yjwei@cn.fujitsu.com>
//
// Any bugs reported given to us we will try to fix... any fixes shared will
// be incorporated into the next SCTP release.
//

#if !defined(__MmSCTPChecksum_h__)
#define __MmSCTPChecksum_h__

#include "MmObject.h"
#include "RObject.h"
#include "WObject.h"

///////////////////////////////////////////////////////////////////////////////

class MmSCTPChecksum : public MmUint{
public:
	MmSCTPChecksum(CSTR s,uint16_t w,const ICVoverwriter* ow=0);
virtual ~MmSCTPChecksum();
public:
// COMPOSE/REVERSE INTERFACE --------------------------------------------------
virtual RObject* reverseRm(RControl&,RObject* r_parent,
		const ItPosition& at,const ItPosition& size,PvObject* pv)const;
virtual WObject* composeWm(WControl& c,WObject* w_parent,
		const PObject* po)const;
virtual	RObject* reverse(RControl&,RObject* r_parent,
		ItPosition& at,OCTBUF& buf)const;
virtual bool geneAuto(WControl& c,WObject* w_self,OCTBUF& buf)const;
//
virtual void add_post(Con_IPinfo* info,TObject* self)const;
};

///////////////////////////////////////////////////////////////////////////////
class RmSCTPChecksum : public RmObject{
	PvObject*	calc_pvalue_;
public:
	RmSCTPChecksum(RObject* r_parent,const MObject* m,
		const ItPosition& offset,const ItPosition& size,PvObject* pv);
virtual	~RmSCTPChecksum();
	void set_calc_pvalue(PvObject* calc);
	PvObject* calc_pvalue()const{return calc_pvalue_;}
virtual void post_reverse(Con_IPinfo& info,RControl&,RObject* base);
virtual void printName(uint32_t t,CSTR cls) const ;
virtual void logSelf(uint32_t t,CSTR cls) const ;
};

///////////////////////////////////////////////////////////////////////////////
class WmSCTPChecksum : public WmObject{
public:
	WmSCTPChecksum(WObject* p,const MObject* m,const PObject* po);
virtual ~WmSCTPChecksum();
virtual void post_generate(Con_IPinfo&,WControl&,OCTBUF& buf,WObject* from);
virtual bool doEvaluate(WControl& c,RObject& r);
};

#endif
