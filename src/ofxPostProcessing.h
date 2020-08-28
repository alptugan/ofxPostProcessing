/*
 *  ofxPostProcessing.h
 *
 *  Copyright (c) 2013, Neil Mendoza, http://www.neilmendoza.com
 *  All rights reserved. 
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met: 
 *  
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer. 
 *  * Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *  * Neither the name of Neil Mendoza nor the names of its contributors may be used 
 *    to endorse or promote products derived from this software without 
 *    specific prior written permission. 
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 *  POSSIBILITY OF SUCH DAMAGE. 
 *
 */
#pragma once

#include "BloomPass.h"
#include "ConvolutionPass.h"
#include "DofPass.h"
#include "DofAltPass.h"
#include "EdgePass.h"
#include "FxaaPass.h"
#include "KaleidoscopePass.h"
#include "NoiseWarpPass.h"
#include "PixelatePass.h"
#include "PostProcessing.h"
#include "RenderPass.h"
#include "LUTPass.h"
#include "ContrastPass.h"
#include "SSAOPass.h"
#include "HorizontalTiltShifPass.h"
#include "VerticalTiltShifPass.h"
#include "RGBShiftPass.h"
#include "FakeSSSPass.h"
#include "ZoomBlurPass.h"
#include "BleachBypassPass.h"
#include "ToonPass.h"
#include "GodRaysPass.h"
#include "RimHighlightingPass.h"
#include "LimbDarkeningPass.h"
// Added by alptugan
#include "FilmGrainLinesPass.hpp"
#include "DotScreenPass.hpp"
#include "DigitalGlitchPass.h"
#include "FirePass.h"
#include "BadTVPass.h"
#include "ACESFilmicToneMappingPass.h"
#include "NoisePass.h"
#include "TiltShiftPass.h"
#include "SuperPass.h"
#include "GlitchAutomatedPass.h"
#include "SpaceColorMovePass.h"
#include "DitherPass.h"

typedef itg::PostProcessing ofxPostProcessing;

using namespace itg;
