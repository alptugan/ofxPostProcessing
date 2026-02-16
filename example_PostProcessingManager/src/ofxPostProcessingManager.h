#pragma once

#include <list>
#include <string>
#include <vector>

//#include "ofMain.h"
#include "ofxPostProcessing.h"
#include "ofxDC_Utilities.h"
#include "ofxGui.h"

class ofxPostProcessingManager{

public:

    void setup(int w, int h, std::string fontName = "", int fontSize = 8);

    void updateValues();

    void drawGui(int x, int y);
    void hideGui();
    void setupGui(std::string fontName, int fontSize);

    void begin();
    void begin(ofCamera& cam);

    void end();

    void windowResized(int w, int h);

    void saveSettings(std::string fileName);
    void loadSettings(std::string fileName = "");

    void saveSessionState();
    void loadSessionState();

    void resetAllEffectsToDefaults();

    // DISABLE ALL EFFECTS
    void disableAll();

    // SWITCH BETWEEN EFFECTS
    void switchFX(int postId);

    // GETTER & SETTERS
    int getEffectNum();
    int getGUIWidth();
    glm::vec2 getGUIPos();

    ofxDC_Utilities utils;


    FxaaPass::Ptr fxaa;
    BloomPass::Ptr bloomPass;
    KaleidoscopePass::Ptr kaleidoscope;
    NoiseWarpPass::Ptr noiseWrap;
    PixelatePass::Ptr pixelate;
    GodRaysPass::Ptr godRays;
    LimbDarkeningPass::Ptr limbDarkening;
    SSAOPass::Ptr ssao;
    ZoomBlurPass::Ptr zoomBlur;

    RGBShiftPass::Ptr rgbPass;

    FilmGrainLinesPass::Ptr filmGrainLinesPass;
    DotScreenPass::Ptr dotScreenPass;
    DigitalGlitchPass::Ptr glitchPass;

    BadTVPass::Ptr badTv;
    ACESFilmicToneMappingPass::Ptr colorACES;
    NoisePass::Ptr noiseGrain;
    TiltShiftPass::Ptr tiltShift;
    SuperPass::Ptr superShader;
    GlitchAutomatedPass::Ptr glitchAuto;
    SpaceColorMovePass::Ptr spaceColor;
    DitherPass::Ptr dither;
    ColorInvert::Ptr strobber;

    RimHighlightingPass::Ptr rimbShift;

    DofPass::Ptr dof;

    // Becareful when enabling a pass...
    //ContrastPass::Ptr contrastShift;
    //FogPass::Ptr fogFilter;
    //DofPass::Ptr dof;
    //FirePass::Ptr lavaFire;
    //SlantShiftPass::Ptr slantShift;
private:

    void reInit();
    void applyEffectStates(bool forceDisable = false);
    void updateEffectGroupStyle(std::size_t index);
    void resetEffectDefaults(std::size_t index);
    std::string getSessionFilePath() const;

    ofxPostProcessing post;

    //gui------------------------------------------

    ofxPanel gui;
    ofxToggle gDebugDraw;


    // LOAD SeTTINGS
    void processOpenFileSelection(ofFileDialogResult openFileResult);
    void savePresetPressed();
    void loadPreset();
    void paramChangedEvent(ofAbstractParameter &e);
    ofParameter<void> btnLoad = {"LOAD SETTINGS"};
    ofParameter<void> btnSave = {"SAVE PRESET"};
    ofParameter<bool> btnRandomize = {"RANDOM FX", false}; // PICK RANDOM FX
    ofParameter<float> sliderTime = {"FX SWITCH PERIOD", 0.1, 0.001 , 1}; // RANDOM SWITCH TIME

    //ofParameter<string> btnFileName = {"PRESET NAME", "..."};
    ofParameter<void> gdisableAll = {"DISABLE ALL FX"};
    ofParameter<bool> gBypassAll = {"BYPASS ALL FX", false};
    ofParameter<void> gResetAll = {"RESET ALL FX"};

    void gdisableAllHandler();
    void gBypassAllHandler(bool & val);

    std::vector<ofxToggle> gBtnEffSwtich;
    std::vector<ofParameterGroup> gEffectGroups; // Parameter grouplar tanımlanacak
    std::vector<ofParameter<void>> gBtnResetEffect;
    std::list<ofEventListener> eResetEffect;
    ofEventListener eResetAll;
    ofEventListener eBypassAll;
    std::vector<bool> bypassSnapshot_;
    bool hasBypassSnapshot_ = false;
    void gBtnEffSwtichHandler(bool & val);

    std::string fileName;


    // 0 - FXAA
    ofParameterGroup gFxaaGroup;
    ofxFloatSlider gFxaaDivMin;
    ofxFloatSlider gFxaaDivMul;
    ofxFloatSlider gFxaaSpanMax;

    // 1 - BLOOM
    ofParameterGroup gBloomGroup;
    ofxFloatSlider gBloomBlurX, gBloomBlurY; 

    // 2 - KALEIDOSCOPE
    ofParameterGroup gKaliGroup;
    ofxFloatSlider gKaleiSegments;

    // 3 - NOISEWRAP
    ofParameterGroup gNoiseWrapGroup;
    ofxFloatSlider gNoiseWrapAmp, gNoiseWrapFreq, gNoiseWrapSpeed;

    // 4- PIXELATE
    ofParameterGroup gPixelateGroup;
    ofxIntSlider gPixelateX, gPixelateY;

    // 5- GODRAYS
    ofParameterGroup gGodRaysGroup;
    ofxFloatSlider gGodRaysLightDotView;
    ofxVec3Slider gLightPositionOnScreen;

    // 6- LIMB DARKENING
    ofParameterGroup gLimb;
    ofxVec3Slider gLimbStartColor, gLimbEndColor;
    ofxFloatSlider gLimbRadScale, gLimbBrightness;

    // 7- SSAO
    ofParameterGroup gSsaoGroup;
    ofxFloatSlider gSsaoAoClamp, gSsaoLumInfluence, gSsaocameraNear, gSsaocameraFar, gSsaofogNear, gSsaofogFar;
    ofxToggle gSsaofogEnabled, gSsaoonlyAO;

    // 8- ZOOM BLUR
    ofParameterGroup gZoomBlurGroup;
    ofxVec2Slider gZoomCenterXY;
    ofxFloatSlider gZoomClamp, gZoomDecay, gZoomWeight, gZoomDensity, gZoomExposure;


    // 9- RGB SHIFT PASS
    ofParameterGroup gRGBGroup;
    ofxFloatSlider gRGBAmount, gRGBAngle;

    // 10- FILM GRAIN
    ofParameterGroup gFilmGrainGroup;
    ofxFloatSlider gFilmGrainLnIntensity,gFilmGrainLsIntensity, gFilmGrainLCount;
    ofxToggle gFilmGrainLGrayScale;

    // 11- DOT SCREEN
    ofParameterGroup gDotScreenGroup;
    ofxFloatSlider gDotScrAngle, gDotScrScale;
    ofxVec2Slider gDotScrSize, gDotScrCenter;

    // 12- GLITCH
    ofParameterGroup gGlicthGroup;
    ofxFloatSlider gGlitchAmount, gGlitchAngle, gGlitchSeed, gGlitchSeedX, gGlitchSeedY, gGlitchDistX, gGlitchDistY, gGlitchCol;
    ofxToggle gGlitchByp;

    // LAVA FIRE


    // 13- BAD TV PASS
    ofParameterGroup gBadTVGroup;
    ofxFloatSlider gBadTvDist,gBadTvDist2,gBadTvSpeed,gBadTvRoll;

    // 14- colorACES
    ofParameterGroup gcolorACESGroup;
    ofxFloatSlider gcolorACESExp;

    // 15- Noise Grain
    ofParameterGroup gNoiseGroup;
    ofxFloatSlider gNoiseSpeed, gNoiseAmt;

    // 16- TiltShift
    ofParameterGroup gTiltShiftGroup;
    ofxFloatSlider gTiltFocus, gTitltRange, gTiltOffset, gTiltStrength;

    // 17- SuperShader
    ofParameterGroup gSupGroup;
    ofxFloatSlider gSupGlowAmt, gSupGlowSize, gSupVigOff, gSupVigDark, gSupBri, gSupCont, gSupSat, gRGBShfAmt;

    // 18- Glitch Automated
    ofParameterGroup gGliAutoGroup;
    ofxFloatSlider gGliAutoSpeed,gGliAutoAmt;

    // 19- Space Color
    ofParameterGroup gSpaceColorGroup;
    ofxFloatSlider gSpaceColorSpeed, gSpaceColorOpacity;

    // 20- Dither
    ofParameterGroup gDitherGroup;
    ofxFloatSlider gDitherScale;

    // 21- Color Invert Strobber
    ofParameterGroup gStrobberGroup;
    ofxFloatSlider gStrobberVolume;
    ofxFloatSlider gStrobberPhase;

    // 22- SLANT SHIFT PASS
    ofParameterGroup gSlantShiftGroup;
    ofxFloatSlider gSlantShiftVolume;

    // 23- RIMB LIGHT SHIFT
    ofParameterGroup gRimbLightGroup;
    ofxVec3Slider gRimbCol;
    ofxFloatSlider gRimbThres;

    // 24- DOF
    ofParameterGroup gDofGroup;
    ofxFloatSlider gDofAperture, gDofMaxBlur, gDofFocus;


    // FOG PASS
    /*ofParameterGroup gFogGroup;
    ofxFloatSlider gFogfogStart, gFogfogEnd;
    ofxVec4Slider gFogfogColor;*/
};


//0-Fxaa
//1-Bloom
//2-Kaleidoscope
//3-Noisewarp
//4-Pixelate
//5-GodRays
//6-LimbDarkening
//7-Ssao
//8-ZoomBlur
//9-gRGBShiftPass
//10-FilmGrainLines
//11-DotScreen
//12-Glitch
//13-Bad TV
//14-Color ACES Filmic Filter
//15-Noise Grains Filter
//16-TiltShift Filter
//17-SuperShader
//18-glitch Automated Filter
//19-gSpaceColor
//20-Dither Pass
//21-Color Invert Strobber
//22-Rimblight
//23-FogFilter
