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
 * $TAHI: v6eval/bin/checker/checker.cc,v 1.12 2001/10/05 11:09:54 tanaka Exp $
 */
#include <stdio.h>
#include "PvName.h"
#include "PcObject.h"
#include "LxLexer.h"
#include "McObject.h"
#include "McNull.h"
#include "McEther.h"
#include "WObject.h"
#include "PvOctets.h"
#include "CmMain.h"
#include "RunEnv.h"
#include "PControl.h"
#include "PAlgorithm.h"

void geneFrames(NameSet& set) {
	BtArray l(set);
	uint32_t i,i9=l.size();
	for(i=0;i<i9;i++) {
		NameDefine* n=(NameDefine*)l[i];
		PcObject* o=n!=0?n->declaration():0;
		if(o==0) continue;
		printf("log:==========================================\n");
		printf("log:Frame %s\n", o->nameString());
		printf("log:------------------------------------------\n");
		SAOctets* saoct=o->generateFrame();
		if(saoct!=0){
			PvOctets* pv=saoct->frame();
			ItPosition rit;
			RSAControl rc(saoct->algorithms());
			McFrame* mcf=McFrame_Ether::instance();
			RObject* ro=mcf->reverse(rc,0,rit,*pv);
			ro->print(0);
			delete ro;}
		delete saoct;}}

void evalFrames(NameSet& set) {
	AlgorithmFrames af;
	BtArray l(set);
	uint32_t i,i9=l.size();
	for(i=0;i<i9;i++) {
		NameDefine* n=(NameDefine*)l[i];
		PcObject* o=n!=0?n->declaration():0;
		if(o==0) continue;
		(void)o->evaluatePrologue(af);}}

applMain() {
	bool ok=false;
	StringList pos;
	CSTR defs=RunEnv::packetDefinition(*main,pos,ok);
	if(!ok) {exit(1);}
	LxLexer file(defs);
	McObject::initialize();
	//--------------------------------------------------------------
	// PARSE PACKET DEFINITION FILE
	parse(file); file.close();
	if(file.errorCount()>0) {exit(1);}
	NameDefine::printUndefines();
	//--------------------------------------------------------------
	// GENERATE SPECIFIED PACKETS
	NameSet& set=PcObject::frameSet();
	geneFrames(set);
	evalFrames(set);
	if(file.errorCount()>0) {exit(1);}
	exit(0);}
