/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008
 * Yokogawa Electric Corporation,
 * YDC Corporation, IPA (Information-technology Promotion Agency, Japan).
 * All rights reserved.
 * 
 * Redistribution and use of this software in source and binary forms, with 
 * or without modification, are permitted provided that the following 
 * conditions and disclaimer are agreed and accepted by the user:
 * 
 * 1. Redistributions of source code must retain the above copyright 
 * notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the 
 * documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the names of the copyrighters, the name of the project which 
 * is related to this software (hereinafter referred to as "project") nor 
 * the names of the contributors may be used to endorse or promote products 
 * derived from this software without specific prior written permission.
 * 
 * 4. No merchantable use may be permitted without prior written 
 * notification to the copyrighters. However, using this software for the 
 * purpose of testing or evaluating any products including merchantable 
 * products may be permitted without any notification to the copyrighters.
 * 
 * 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHTERS, THE PROJECT AND 
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING 
 * BUT NOT LIMITED THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE, ARE DISCLAIMED.  IN NO EVENT SHALL THE 
 * COPYRIGHTERS, THE PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT,STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $TAHI: v6eval/lib/pkt/RAFControl.h,v 1.7 2008/04/02 00:00:13 akisada Exp $
 */
#if !defined(__RAFControl_h__)
#define	__RAFControl_h__	1
#include "PControl.h"
class McObject;
class AlgorithmFrames;
class WAlgorithm;
class WObjectList;
class PaObject;
class RObject;
class WObject;
//======================================================================
class RAFControl:public RControl {
private:
	const AlgorithmFrames& candidates_;
	uint32_t current_;
	const WAlgorithm* algorithm_;
public:
	RAFControl(const AlgorithmFrames&);
	~RAFControl();
	const AlgorithmFrames& candidates() const;
	const WAlgorithm* algorithm() const;
	void algorithm(const WAlgorithm*);
	const WObjectList& noneAlgorithms() const;
	bool nextAlgorithm();
virtual	const PaObject*	pop_SA(eAlgorithm,uint32_t =0,const PvObject* =0);
	CSTR reverseEvaluate(const McObject&,PvOctets&,bool&);
	bool evaluateWith(RObject*,WObject*) const;
	WObject* evaluateAlgorithm(RObject*) const;
	WObject* evaluate(RObject*);
virtual	void reset();
	void applyFrame(const WObject* w) const;
	const WAlgorithm* appliedAlgorithm() const;
};
inline const WAlgorithm* RAFControl::algorithm() const {
	return algorithm_;}
inline const AlgorithmFrames& RAFControl::candidates() const {
	return candidates_;}
inline void RAFControl::algorithm(const WAlgorithm* w) {
	algorithm_=w;}
#endif
