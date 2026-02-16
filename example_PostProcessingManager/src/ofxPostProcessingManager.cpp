
#include <algorithm>

#include <ofMain.h>

#include "ofxPostProcessingManager.h"

void ofxPostProcessingManager::processOpenFileSelection(ofFileDialogResult openFileResult) {
    ofLogVerbose("getName(): "  + openFileResult.getName());
    ofLogVerbose("getPath(): "  + openFileResult.getPath());

    ofFile file (openFileResult.getPath());

    if (file.exists()){

        ofLogVerbose("The file exists - now checking the type via file extension");
        std::string fileExtension = ofToUpper(file.getExtension());

        //We only want images
        if (fileExtension == "JSON") {
            loadSettings(openFileResult.getPath());
        }
    }
}

void ofxPostProcessingManager::loadPreset() {
    ofFileDialogResult openFileResult= ofSystemLoadDialog("Select fxSettings .json file");

    //Check if the user opened a file
    if (openFileResult.bSuccess){

        ofLogVerbose("User selected a file");

        //We have a file, check it and process it
        processOpenFileSelection(openFileResult);

    }else {
        ofLogVerbose("User hit cancel");
    }
}

void ofxPostProcessingManager::savePresetPressed() {
    //Open the Open File Dialog
    ofFileDialogResult saveFileResult = ofSystemSaveDialog("fxSettings_"+ofGetTimestampString() + ".json", "Save your file");
    if (saveFileResult.bSuccess){
        ofLog() << "FX Preset Saved As " << saveFileResult.filePath;
        gui.saveToFile(saveFileResult.filePath);
    }

}

int ofxPostProcessingManager::getEffectNum() {
    return post.getPasses().size();
}

void ofxPostProcessingManager::saveSettings(std::string fileName) {
    gui.saveToFile(fileName);
    ofLog() << "------------------------------------------------";
    ofLog() << "Settings saved as " << fileName;
}


void ofxPostProcessingManager::loadSettings(std::string fileName) {
    if(fileName == "") {
        fileName = this->fileName;
        gui.loadFromFile(this->fileName);
    }else{
        gui.loadFromFile(fileName);
    }
    ofLogNotice() << "------------------------------------------------";
    ofLogNotice() << "Settings loaded as " << fileName;
    ofLogNotice() << "------------------------------------------------";
}

std::string ofxPostProcessingManager::getSessionFilePath() const {
    const std::string dirPath = ofToDataPath("fx", true);
    ofDirectory dir(dirPath);
    if (!dir.exists()) {
        dir.create(true);
    }
    return ofFilePath::join(dirPath, "fx_session.json");
}

void ofxPostProcessingManager::saveSessionState() {
    if (gBtnEffSwtich.empty()) {
        return;
    }

    const std::string path = getSessionFilePath();
    gui.saveToFile(path);
    ofLogNotice() << "FX session saved: " << path;
}

void ofxPostProcessingManager::loadSessionState() {
    const std::string path = getSessionFilePath();
    ofFile file(path);
    if (!file.exists()) {
        return;
    }

    gui.loadFromFile(path);

    if (gBypassAll.get()) {
        bypassSnapshot_.assign(gBtnEffSwtich.size(), false);
        for (std::size_t i = 0; i < gBtnEffSwtich.size(); ++i) {
            bypassSnapshot_[i] = gBtnEffSwtich[i];
        }
        hasBypassSnapshot_ = true;
        applyEffectStates(true);
    } else {
        hasBypassSnapshot_ = false;
        applyEffectStates(false);
    }

    ofLogNotice() << "FX session loaded: " << path;
}

void ofxPostProcessingManager::applyEffectStates(bool forceDisable) {
    for (std::size_t i = 0; i < static_cast<std::size_t>(post.size()); ++i) {
        const bool isEnabled = (!forceDisable) && gBtnEffSwtich[i];
        post[i]->setEnabled(isEnabled);
        updateEffectGroupStyle(i);
    }
}

void ofxPostProcessingManager::updateEffectGroupStyle(std::size_t index) {
    const std::string name = gBtnEffSwtich[index].getName();
    if (gBtnEffSwtich[index] == true) {
        gui.getGroup(name).setHeaderBackgroundColor(ofColor(0,200,0,150));
        gui.getGroup(name).setBorderColor(ofColor(0,200,0,150));
        gui.getGroup(name).setTextColor(ofColor(0,0,0,150));
    } else {
        gui.getGroup(name).setHeaderBackgroundColor(ofColor(0,0,0,100));
        gui.getGroup(name).setBorderColor(ofColor(0,0,0,100));
        gui.getGroup(name).setTextColor(ofColor(255,255,255,150));
    }
}

void ofxPostProcessingManager::resetAllEffectsToDefaults() {
    for (std::size_t i = 0; i < gBtnEffSwtich.size(); ++i) {
        resetEffectDefaults(i);
    }
}

void ofxPostProcessingManager::resetEffectDefaults(std::size_t index) {
    switch (index) {
        case 0:
            gFxaaDivMin = 128.0f;
            gFxaaDivMul = 8.0f;
            gFxaaSpanMax = 8.0f;
            break;
        case 1:
            gBloomBlurX = 0.000953125f;
            gBloomBlurY = 0.000953125f;
            break;
        case 2:
            gKaleiSegments = 2.0f;
            break;
        case 3:
            gNoiseWrapAmp = 0.1f;
            gNoiseWrapFreq = 4.0f;
            gNoiseWrapSpeed = 0.1f;
            break;
        case 4:
            gPixelateX = 100;
            gPixelateY = 100;
            break;
        case 5:
            gGodRaysLightDotView = 0.3f;
            gLightPositionOnScreen = glm::vec3(0.0f);
            break;
        case 6:
            gLimbRadScale = 1.2f;
            gLimbStartColor = glm::vec3(1.0f, 0.0f, 1.0f);
            gLimbEndColor = glm::vec3(0.0f, 0.0f, 1.0f);
            break;
        case 7:
            gSsaofogEnabled = false;
            gSsaofogNear = 1.0f;
            gSsaofogFar = 1000.0f;
            gSsaocameraNear = 1.0f;
            gSsaocameraFar = 1000.0f;
            gSsaoonlyAO = false;
            gSsaoAoClamp = 0.65f;
            gSsaoLumInfluence = 0.25f;
            break;
        case 8:
            gZoomCenterXY = glm::vec2(0.5f, 0.5f);
            gZoomWeight = 0.25f;
            gZoomDensity = 0.25f;
            gZoomExposure = 0.48f;
            break;
        case 9:
            gRGBAngle = 0.0f;
            gRGBAmount = 0.005f;
            break;
        case 10:
            gFilmGrainLCount = 1096.0f;
            gFilmGrainLGrayScale = false;
            gFilmGrainLnIntensity = 0.5f;
            gFilmGrainLsIntensity = 0.5f;
            break;
        case 11:
            gDotScrCenter = glm::vec2(0.5f, 0.5f);
            gDotScrSize = glm::vec2(2000.0f);
            gDotScrScale = 1.0f;
            gDotScrAngle = 1.57f;
            break;
        case 12:
            gGlitchAmount = 0.006f;
            gGlitchByp = false;
            gGlitchAngle = 0.02f;
            gGlitchSeed = 0.02f;
            gGlitchSeedX = 0.02f;
            gGlitchSeedY = 0.02f;
            gGlitchDistX = 0.02f;
            gGlitchDistY = 0.02f;
            gGlitchCol = 0.03f;
            break;
        case 13:
            gBadTvDist = 3.0f;
            gBadTvDist2 = 5.0f;
            gBadTvSpeed = 3.0f;
            gBadTvRoll = 0.1f;
            break;
        case 14:
            gcolorACESExp = 1.0f;
            break;
        case 15:
            gNoiseAmt = 0.128f;
            gNoiseSpeed = 0.08f;
            break;
        case 16:
            gTiltFocus = 0.35f;
            gTitltRange = 0.5f;
            gTiltOffset = 0.05f;
            gTiltStrength = 0.5f;
            break;
        case 17:
            gSupGlowAmt = 0.5f;
            gSupGlowSize = 4.0f;
            gSupVigOff = 1.0f;
            gSupVigDark = 1.0f;
            gSupBri = 0.0f;
            gSupCont = 0.0f;
            gSupSat = 0.0f;
            gRGBShfAmt = 0.01f;
            break;
        case 18:
            gGliAutoSpeed = 0.6f;
            gGliAutoAmt = 0.2f;
            break;
        case 19:
            gSpaceColorSpeed = 1.0f;
            gSpaceColorOpacity = 0.1f;
            break;
        case 20:
            gDitherScale = 1.0f;
            break;
        case 21:
            gStrobberVolume = 1.0f;
            gStrobberPhase = 0.15f;
            break;
        case 22:
            gRimbCol = glm::vec3(2.9f, 1.3f, 1.3f);
            gRimbThres = 64.0f;
            break;
        case 23:
            gDofAperture = 0.8f;
            gDofFocus = 0.985f;
            gDofMaxBlur = 0.6f;
            break;
        default:
            break;
    }
}

//---------------------------------------
void ofxPostProcessingManager::setup(int w, int h, std::string f, int fSize){

    post.init(w, h);

    reInit();

    //---------
    setupGui(f, fSize);
}

void ofxPostProcessingManager::reInit() {

    //6-EdgePass
    //post.createPass<EdgePass>()->setEnabled(false);

    //7-VerticalTilt
    //post.createPass<VerticalTiltShifPass>()->setEnabled(false);

    //16-Fire Lava
    //lavaFire = post.createPass<FirePass>();

    /*gDoEdgePass = false;
     gDoVerticalTiltShift = false;*/


    //0-Fxaa
    fxaa = post.createPass<FxaaPass>();

    //1-Bloom
    bloomPass = post.createPass<BloomPass>();

    //2-Kaleidoscope
    kaleidoscope = post.createPass<KaleidoscopePass>();

    //3-Noisewarp
    noiseWrap = post.createPass<NoiseWarpPass>();


    //4-Pixelate
    pixelate = post.createPass<PixelatePass>();

    //5-GodRays
    godRays = post.createPass<GodRaysPass>();

    //6-LimbDarkening
    limbDarkening=post.createPass<LimbDarkeningPass>();

    //7-Ssao
    ssao=post.createPass<SSAOPass>();

    //8-ZoomBlur
    zoomBlur = post.createPass<ZoomBlurPass>();

    //9-gRGBShiftPass
    rgbPass = post.createPass<RGBShiftPass>();

    //10-FilmGrainLines
    filmGrainLinesPass = post.createPass<FilmGrainLinesPass>();

    //11-DotScreen
    dotScreenPass = post.createPass<DotScreenPass>();

    //12-Glitch
    glitchPass = post.createPass<DigitalGlitchPass>();

    //13-Bad TV
    badTv = post.createPass<BadTVPass>();

    //14-Color ACES Filmic Filter
    colorACES = post.createPass<ACESFilmicToneMappingPass>();

    //15-Noise Grains Filter
    noiseGrain = post.createPass<NoisePass>();

    //16-TiltShift Filter
    tiltShift = post.createPass<TiltShiftPass>();

    //17-SuperShader
    superShader = post.createPass<SuperPass>();

    //18-glitch Automated Filter
    glitchAuto = post.createPass<GlitchAutomatedPass>();

    //19-gSpaceColor
    spaceColor = post.createPass<SpaceColorMovePass>();

    //20-Dither Pass
    dither = post.createPass<DitherPass>();

    //21-Color Invert Strobber
    strobber = post.createPass<ColorInvert>();

    // 22-Rimblight Pass
    rimbShift = post.createPass<RimHighlightingPass>();

    // 23-DOF -> Depth of Field
    dof = post.createPass<DofPass>();

    //22-SlantShift Pass
    //slantShift = post.createPass<SlantShiftPass>();
    // 24-Fog Filter Pass
    //fogFilter = post.createPass<FogPass>();
}
//---------------------------------------
// SWTICH EFFECT
//---------------------------------------
void ofxPostProcessingManager::switchFX(int postId) {
    gBtnEffSwtich[postId] = !gBtnEffSwtich[postId];
    applyEffectStates(gBypassAll.get());
}

//---------------------------------------
// UPDATE EFFECT PARAMETERS
//---------------------------------------
void ofxPostProcessingManager::updateValues(){


    //---------------------------------------
    // RANDOMIZE EFFECT
    //---------------------------------------
    if(btnRandomize)
    {
        if(utils.notifyPerSecond(sliderTime)) {
            disableAll();
            switchFX(floor(ofRandom(getEffectNum())));
        }
    }
    // 16- Lava Fire Pass
    //if(post[16]->getEnabled()) {
    //}
    // 0 - FXAA
    if(post[0]->getEnabled()) {
        fxaa->setDivMin(gFxaaDivMin);
        fxaa->setDivMul(gFxaaDivMul);
        fxaa->setSpanMax(gFxaaSpanMax);
    }

    // 1- Bloom
    if(post[1]->getEnabled())
        bloomPass->setBlurXY(gBloomBlurX, gBloomBlurY);

    // 2- Kaleidoscope
    if(post[2]->getEnabled())
        kaleidoscope->setSegments(gKaleiSegments);

    // 3- NoiseWrap
    if(post[3]->getEnabled()) {
        noiseWrap->setAmplitude(gNoiseWrapAmp);
        noiseWrap->setFrequency(gNoiseWrapFreq);
        noiseWrap->setSpeed(gNoiseWrapSpeed);
    }

    // 4- Pixelate
    if(post[4]->getEnabled()) {
        pixelate->setResolution(gPixelateX, gPixelateY);
    }

    // 5- GOD Rays
    if(post[5]->getEnabled()) {
        godRays->setLightDirDOTviewDir(gGodRaysLightDotView);
        godRays->setLightPositionOnScreen(gLightPositionOnScreen);
    }

    // 6- Limb Darkening
    if(post[6]->getEnabled()) {
        limbDarkening->setBrightness(gLimbBrightness);
        limbDarkening->setRadialScale(gLimbRadScale);
        limbDarkening->setStartColor(gLimbStartColor);
        limbDarkening->setEndColor(gLimbEndColor);
    }

    // 7- SSAO
    if(post[7]->getEnabled()) {
        ssao->setFogEnabled(gSsaofogEnabled);
        ssao->setFogNear(gSsaofogNear);
        ssao->setFogFar(gSsaofogFar);
        ssao->setCameraNear(gSsaocameraNear);
        ssao->setCameraFar(gSsaocameraFar);
        ssao->setOnlyAO(gSsaoonlyAO);
        ssao->setAoClamp(gSsaoAoClamp);
        ssao->setLumInfluence(gSsaoLumInfluence);
    }

    // 8- ZoomBlur
    if(post[8]->getEnabled()) {
        zoomBlur->setCenterX(gZoomCenterXY->x);
        zoomBlur->setCenterY(gZoomCenterXY->y);
        //zoomBlur->setClamp(gZoomClamp);
        //zoomBlur->setDecay(gZoomDecay);
        zoomBlur->setWeight(gZoomWeight);
        zoomBlur->setDensity(gZoomDensity);
        zoomBlur->setExposure(gZoomExposure);
    }

    // 9- RGB Pass
    if(post[9]->getEnabled()) {
        rgbPass->setAngle(gRGBAngle);
        rgbPass->setAmount(gRGBAmount);
    }

    // 10- FilmGrainLines Pass
    if(post[10]->getEnabled()) {
        filmGrainLinesPass->setCount(gFilmGrainLCount);
        filmGrainLinesPass->setGrayScale(gFilmGrainLGrayScale);
        filmGrainLinesPass->setnIntensity(gFilmGrainLnIntensity);
        filmGrainLinesPass->setsIntensity(gFilmGrainLsIntensity);
    }

    // 11- DotScreen Pass
    if(post[11]->getEnabled())  {
        dotScreenPass->setSize(glm::vec2(gDotScrSize->x, gDotScrSize->y));
        dotScreenPass->setAgnle(gDotScrAngle);
        dotScreenPass->setScale(gDotScrScale);
        dotScreenPass->setCenter(glm::vec2(gDotScrCenter->x, gDotScrCenter->y));
    }

    // 12- Glitch Pass
    if(post[12]->getEnabled()) {
        gGlitchCol = ofRandom(0.001, 1);
        gGlitchDistX = ofSignedNoise(ofGetFrameNum() * 0.04);
        gGlitchDistY = ofSignedNoise(ofGetFrameNum() * 0.01);
        gGlitchAngle = ofRandom(0, TWO_PI);

        //glitchPass->setByp(gGlitchByp);
        glitchPass->setCols(gGlitchCol);
        glitchPass->setSeed(gGlitchSeed);
        glitchPass->setSeedX(gGlitchSeedX);
        glitchPass->setSeedY(gGlitchSeedY);
        glitchPass->setDistortionX(gGlitchDistX);
        glitchPass->setDistortionY(gGlitchDistY);
        glitchPass->setAngle(gGlitchAngle);
        glitchPass->setAmount(gGlitchAmount);
    }

    // 13- Bad TV Effect
    if(post[13]->getEnabled()) {
        badTv->setSpeed(gBadTvSpeed);
        badTv->setRollSpeed(gBadTvRoll);
        badTv->setDistortion(gBadTvDist);
        badTv->setDistortion2(gBadTvDist2);
    }

    // 14- COLOR ACES FILMIC FILTER
    if(post[14]->getEnabled()) {
        colorACES->setExposure(gcolorACESExp);
    }

    // 15- Noise Grain Filter
    if(post[15]->getEnabled()) {
        noiseGrain->setSpeed(gNoiseSpeed);
        noiseGrain->setAmount(gNoiseAmt);
    }

    // 16- Tilt Shift
    if(post[16]->getEnabled()) {
        tiltShift->setFocusPos(gTiltFocus);
        tiltShift->setRange(gTitltRange);
        tiltShift->setOffset(gTiltOffset);
        tiltShift->setStrength(gTiltStrength);
    }

    // 17- SuperShader
    if(post[17]->getEnabled()) {
        superShader->setGlowAmount(gSupGlowAmt);
        superShader->setGlowSize(gSupGlowSize);
        superShader->setVigOffset(gSupVigOff);
        superShader->setVigDark(gSupVigDark);
        superShader->setBrightness(gSupBri);
        superShader->setContrast(gSupCont);
        superShader->setSaturation(gSupSat);
        superShader->setRGBShiftAmount(gRGBShfAmt);
    }

    // 18 Automated Glitch
    if(post[18]->getEnabled()) {
        glitchAuto->setSpeed(gGliAutoSpeed);
        glitchAuto->setAmount(gGliAutoAmt);
    }

    // 19 Space Color
    if(post[19]->getEnabled()) {
        spaceColor->setSpeed(gSpaceColorSpeed);
        spaceColor->setOpacity(gSpaceColorOpacity);
    }

    // 20- Dither
    if(post[20]->getEnabled()) {
        dither->setScale(gDitherScale);
    }

    // 21- Color Invert Strobber
    if(post[21]->getEnabled()) {
        if(utils.notifyPerSecond(gStrobberPhase)) {
            gStrobberVolume = 1;
        }else{
            gStrobberVolume = 0;
        }
        strobber->setVolume(gStrobberVolume);
    }

    // 22- Rimblight
    if(post[22]->getEnabled()) {
        rimbShift->setThres(gRimbThres);
        rimbShift->setFloatColor(gRimbCol);
    }

    // 23- DOF
    if(post[23]->getEnabled())
     {
        dof->setAperture(gDofAperture);
        dof->setMaxBlur(gDofMaxBlur);
        dof->setFocus(gDofFocus);
     }

    // 24
    /*if(post[24]->getEnabled()) {
        fogFilter->setColor(gFogfogColor);
        fogFilter->setStart(gFogfogStart);
        fogFilter->setEnd(gFogfogEnd);
    }*/

    /*if(ofGetKeyPressed('g')) {
     gGlitch = !gGlitch;
     }*/
}


//---------------------------------------
void ofxPostProcessingManager::drawGui(int x, int y){

    gui.setPosition(x, y);

    if(gDebugDraw)
        post.debugDraw();

    gui.draw();
}

void ofxPostProcessingManager::hideGui() {
    gui.setPosition(0, 2500);

    gui.draw();
}
//---------------------------------------
void ofxPostProcessingManager::begin(){
    post.begin();
}
//---------------------------------------
void ofxPostProcessingManager::begin(ofCamera& cam){
    post.begin(cam);
}

//---------------------------------------
void ofxPostProcessingManager::end(){
    post.end();
}


void ofxPostProcessingManager::gBtnEffSwtichHandler(bool & val) {
    applyEffectStates(gBypassAll.get());
}

void ofxPostProcessingManager::windowResized(int w, int h) {

    //post.getPasses().clear();
    post.init(w,h);
    //reInit();
}

void ofxPostProcessingManager::disableAll() {
    for (int i = 0; i < post.size(); i++) {
        gBtnEffSwtich[i] = false;
    }
    applyEffectStates(gBypassAll.get());
}

void ofxPostProcessingManager::gdisableAllHandler() {
    disableAll();
}

void ofxPostProcessingManager::gBypassAllHandler(bool & val) {
    if (val) {
        bypassSnapshot_.assign(gBtnEffSwtich.size(), false);
        for (std::size_t i = 0; i < gBtnEffSwtich.size(); ++i) {
            bypassSnapshot_[i] = gBtnEffSwtich[i];
        }
        hasBypassSnapshot_ = true;
        applyEffectStates(true);
    } else {
        if (hasBypassSnapshot_) {
            const std::size_t count = std::min(gBtnEffSwtich.size(), bypassSnapshot_.size());
            for (std::size_t i = 0; i < count; ++i) {
                gBtnEffSwtich[i] = bypassSnapshot_[i];
            }
        }
        applyEffectStates(false);
    }
}

//---------------------------------------
void ofxPostProcessingManager::setupGui(std::string f, int fSize){
    fileName = "fxSettings.json";

	// bypass ui styling
    #if 1
    ofFile file(f);
    // Set the fill color
    ofxGuiSetFillColor(ofColor(0,200,0,130));
    gui.setDefaultTextPadding(7);
    gui.setDefaultWidth(220);
    gui.setDefaultHeight(12);
    if(f != "" && file.exists()) {
       if(file.exists()) {
           gui.loadFont(f, fSize, false);
       }else{
           ofLogError(__FUNCTION__) << "file " << f << " not found!";
       }
    }
    #endif

    gui.setup("POST PROCESSING FX", fileName);
    gui.add(gBypassAll);
    gui.add(gdisableAll);
    gui.add(gResetAll);
    gui.add(btnLoad);
    //gui.add(btnFileName );
    gui.add(btnSave);
    gui.add(btnRandomize);
    gui.add(sliderTime);
    gui.add(gDebugDraw.setup("DEBUG DRAW", false));

    for (int i = 0; i < post.size(); i++) {
        gBtnEffSwtich.push_back(ofxToggle());
        gBtnEffSwtich[i].setup(ofToString(i) + " " + post[i]->getName(), false);
        gBtnEffSwtich[i].addListener(this, &ofxPostProcessingManager::gBtnEffSwtichHandler);
    }

    gBtnResetEffect.resize(post.size());
    eResetEffect.clear();
    for (int i = 0; i < post.size(); i++) {
        gBtnResetEffect[i].set("RESET");
        eResetEffect.emplace_back(gBtnResetEffect[i].newListener([this, i](const void * sender) {
            resetEffectDefaults(static_cast<std::size_t>(i));
        }));
    }

    //gui.add(gDoEdgePass.setup("EdgePass", false));

    //gui.add(gDoVerticalTiltShift.setup("VerticalTiltShift", false));

    // 16- Lava Fire

    //0-Fxaa
    int cc = 0;
    gFxaaGroup.setName(ofToString(cc) + " " + fxaa->getName());
    gFxaaGroup.add(gBtnEffSwtich[0].getParameter());
    gFxaaGroup.add(gBtnResetEffect[0]);
    gFxaaGroup.add(gFxaaDivMin.setup("REDUCE MIN DIV", 128.0, 1.0, 512.0 )->getParameter());
    gFxaaGroup.add(gFxaaDivMul.setup("REDUCE MULT DIV", 8.0, 1.0, 128.0 )->getParameter());
    gFxaaGroup.add(gFxaaSpanMax.setup("SPAN MAX", 8.0, 1.0, 128.0 )->getParameter());
    //gui.add(&gBtnEffSwtich[0]);

    cc++;
    //1-Bloom
    gBloomGroup.setName(ofToString(cc) + " " + bloomPass->getName());
    gBloomGroup.add(gBtnEffSwtich[1].getParameter());
    gBloomGroup.add(gBtnResetEffect[1]);
    gBloomGroup.add(gBloomBlurX.setup("Blur X", 0.000953125, 0, 0.01 )->getParameter());
    gBloomGroup.add(gBloomBlurY.setup("Blur Y", 0.000953125, 0, 0.01 )->getParameter());
    //gui.add(&gBtnEffSwtich[1]);

    cc++;
    //2-Kaleidoscope
    gKaliGroup.setName(ofToString(cc) + " " + kaleidoscope->getName());
    gKaliGroup.add(gBtnEffSwtich[2].getParameter());
    gKaliGroup.add(gBtnResetEffect[2]);
    gKaliGroup.add(gKaleiSegments.setup("Segments", 2.f, -20, 20)->getParameter());
    //gui.add(&gBtnEffSwtich[2]);

    cc++;
    //3-Noisewarp
    gNoiseWrapGroup.setName(ofToString(cc) + " " + noiseWrap->getName());
    gNoiseWrapGroup.add(gBtnEffSwtich[3].getParameter());
    gNoiseWrapGroup.add(gBtnResetEffect[3]);
    gNoiseWrapGroup.add(gNoiseWrapAmp.setup("Amplitude", 0.1, 0, 10)->getParameter());
    gNoiseWrapGroup.add(gNoiseWrapFreq.setup("Frequency", 4.0, 0, 20)->getParameter());
    gNoiseWrapGroup.add(gNoiseWrapSpeed.setup("Speed", 0.1, 0, 10)->getParameter());
    //gui.add(&gBtnEffSwtich[3]);

    cc++;
    // 4- Pixelate
    gPixelateGroup.setName(ofToString(cc) + " " + pixelate->getName());
    gPixelateGroup.add(gBtnEffSwtich[4].getParameter());
    gPixelateGroup.add(gBtnResetEffect[4]);
    gPixelateGroup.add(gPixelateX.setup("Horizontal", 100, 0, 1000)->getParameter());
    gPixelateGroup.add(gPixelateY.setup("Vertical", 100, 0, 1000)->getParameter());
    //gui.add(&gBtnEffSwtich[4]);

    cc++;
    //5-GodRays
    gGodRaysGroup.setName(ofToString(cc) + " " + godRays->getName());
    gGodRaysGroup.add(gBtnEffSwtich[5].getParameter());
    gGodRaysGroup.add(gBtnResetEffect[5]);
    gGodRaysGroup.add(gGodRaysLightDotView.setup("GodRays-Light", 0.3, 0.0, 1.0)->getParameter());
    gGodRaysGroup.add(gLightPositionOnScreen.setup("Light Position", glm::vec3(0), glm::vec3(0), glm::vec3(1))->getParameter());
    //gui.add(&gBtnEffSwtich[5]);

    cc++;
    //6-LimbDarkening
    gLimb.setName(ofToString(cc) + " " + limbDarkening->getName());
    gLimb.add(gBtnEffSwtich[6].getParameter());
    gLimb.add(gBtnResetEffect[6]);
    //gLimb.add(gLimbBrightness.setup("Brightness", 2.5, 0, 10)->getParameter());
    gLimb.add(gLimbRadScale.setup("Scale", 1.2, 0, 100)->getParameter());
    gLimb.add(gLimbStartColor.setup("Begin Color", glm::vec3(1.), glm::vec3(0.), glm::vec3(1.))->getParameter());
    gLimb.add(gLimbEndColor.setup("End Color", glm::vec3(0.), glm::vec3(0.), glm::vec3(1.))->getParameter());
    //gui.add(&gBtnEffSwtich[6]);

    cc++;
    //7-Ssao
    gSsaoGroup.setName(ofToString(cc) + " " + ssao->getName());
    gSsaoGroup.add(gBtnEffSwtich[7].getParameter());
    gSsaoGroup.add(gBtnResetEffect[7]);
    gSsaoGroup.add(gSsaofogEnabled.setup("Enable Fog", false)->getParameter());
    gSsaoGroup.add(gSsaofogNear.setup("Fog Near", 1, 0, 2000)->getParameter());
    gSsaoGroup.add(gSsaofogFar.setup("Fog Far", 1000, 0, 2000)->getParameter());
    gSsaoGroup.add(gSsaocameraNear.setup("Camera Near",1, 0, 2000)->getParameter());
    gSsaoGroup.add(gSsaocameraFar.setup("Camera Far",1000, 0, 2000)->getParameter());
    gSsaoGroup.add(gSsaoonlyAO.setup("Enable-Ao", false)->getParameter());
    gSsaoGroup.add(gSsaoAoClamp.setup("Ssao-AoClamp", 0.65, 0.0, 1.0)->getParameter());
    gSsaoGroup.add(gSsaoLumInfluence.setup("Ssao-LumInfluence", 0.25, 0.0, 1.0)->getParameter());
    //gui.add(&gBtnEffSwtich[7]);

    cc++;
    // 8- ZoomBlur
    gZoomBlurGroup.setName(ofToString(cc) + " " + zoomBlur->getName());
    gZoomBlurGroup.add(gBtnEffSwtich[8].getParameter());
    gZoomBlurGroup.add(gBtnResetEffect[8]);
    gZoomBlurGroup.add(gZoomCenterXY.setup("Zoom Center XY", glm::vec2(0.5, 0.5), glm::vec2(0, 0), glm::vec2(1, 1))->getParameter());
    //gui.add(gZoomClamp.setup("Clamp",1,0,1));
    //gui.add(gZoomDecay.setup("Decay",0.9,0,1));
    gZoomBlurGroup.add(gZoomWeight.setup("Weight",0.25,0,1)->getParameter());
    gZoomBlurGroup.add(gZoomDensity.setup("Density",0.25,0,1)->getParameter());
    gZoomBlurGroup.add(gZoomExposure.setup("Exposure",0.48,0,1)->getParameter());
    //gui.add(&gBtnEffSwtich[8]);

    cc++;
    // 9- RGB Pass
    gRGBGroup.setName(ofToString(cc) + " " + rgbPass->getName());
    gRGBGroup.add(gBtnEffSwtich[9].getParameter());
    gRGBGroup.add(gBtnResetEffect[9]);
    gRGBGroup.add(gRGBAngle.setup("Angle", 0, 0, TWO_PI)->getParameter());
    gRGBGroup.add(gRGBAmount.setup("Amount", 0.005,0,1)->getParameter());
    //gui.add(&gBtnEffSwtich[9]);

    cc++;
    // 10- FilmGrainLines Pass Params
    gFilmGrainGroup.setName(ofToString(cc) + " " + filmGrainLinesPass->getName());
    gFilmGrainGroup.add(gBtnEffSwtich[10].getParameter());
    gFilmGrainGroup.add(gBtnResetEffect[10]);
    gFilmGrainGroup.add(gFilmGrainLCount.setup("Count", 1096,1, 10000)->getParameter());
    gFilmGrainGroup.add(gFilmGrainLGrayScale.setup("GrayScale",false)->getParameter());
    gFilmGrainGroup.add(gFilmGrainLnIntensity.setup("N Intensity",0.5,0,1)->getParameter());
    gFilmGrainGroup.add(gFilmGrainLsIntensity.setup("S Intensity",0.5,0,1)->getParameter());
    //gui.add(&gBtnEffSwtich[10]);

    cc++;
    // 11- DotScreen Pass params
    gDotScreenGroup.setName(ofToString(cc) + " " + dotScreenPass->getName());
    gDotScreenGroup.add(gBtnEffSwtich[11].getParameter());
    gDotScreenGroup.add(gBtnResetEffect[11]);
    gDotScreenGroup.add(gDotScrCenter.setup("Center", glm::vec2(0.5), glm::vec2(0), glm::vec2(1.0,1.0))->getParameter());
    gDotScreenGroup.add(gDotScrSize.setup("Size", glm::vec2(2000), glm::vec2(0), glm::vec2(2000))->getParameter());
    gDotScreenGroup.add(gDotScrScale.setup("Scale",1.0,0,1)->getParameter());
    gDotScreenGroup.add(gDotScrAngle.setup("Angle",1.57,0,TWO_PI)->getParameter());
    //gui.add(&gBtnEffSwtich[11]);

    cc++;
    // 12- Digital Glitch Pass Params
    gGlicthGroup.setName(ofToString(cc) + " " + glitchPass->getName());
    gGlicthGroup.add(gBtnEffSwtich[12].getParameter());
    gGlicthGroup.add(gBtnResetEffect[12]);
    gGlicthGroup.add(gGlitchAmount.setup("Amount", 0.006, 0.00001, 0.1)->getParameter());
    gGlicthGroup.add(gGlitchByp.setup("Bypass", false)->getParameter());
    gGlicthGroup.add(gGlitchAngle.setup("Angle", 0.02, 0, TWO_PI)->getParameter());
    gGlicthGroup.add(gGlitchSeed.setup("Seed", 0.02, 0, 1)->getParameter());
    gGlicthGroup.add(gGlitchSeedX.setup("Seed X", 0.02, 0, 1)->getParameter());
    gGlicthGroup.add(gGlitchSeedY.setup("Seed Y", 0.02, 0, 1)->getParameter());
    gGlicthGroup.add(gGlitchDistX.setup("Distortion X", 0.02, -1, 1)->getParameter());
    gGlicthGroup.add(gGlitchDistY.setup("Distortion Y", 0.02, -1, 1)->getParameter());
    gGlicthGroup.add(gGlitchCol.setup("Column", 0.03, 0, 1)->getParameter());
    //gui.add(&gBtnEffSwtich[12]);

    cc++;
    // 13- Bad TV Pass Params
    gBadTVGroup.setName(ofToString(cc) + " " + badTv->getName());
    gBadTVGroup.add(gBtnEffSwtich[13].getParameter());
    gBadTVGroup.add(gBtnResetEffect[13]);
    gBadTVGroup.add(gBadTvDist.setup("Distortion 1", 3.0, 0.0, 20.0)->getParameter());
    gBadTVGroup.add(gBadTvDist2.setup("Distortion 2", 5.0, 0.0, 20.0)->getParameter());
    gBadTVGroup.add(gBadTvSpeed.setup("Speed", 3.0, 0.0, 20.0)->getParameter());
    gBadTVGroup.add(gBadTvRoll.setup("Roll Speed", 0.1, 0.0, 1.0)->getParameter());
    //gui.add(&gBtnEffSwtich[13]);

    cc++;
    // 14- Color ACES Filmic Filter Paramas
    gcolorACESGroup.setName(ofToString(cc) + " " + colorACES->getName());
    gcolorACESGroup.add(gBtnEffSwtich[14].getParameter());
    gcolorACESGroup.add(gBtnResetEffect[14]);
    gcolorACESGroup.add(gcolorACESExp.setup("Exposure", 1.0, 0.0, 1.0)->getParameter());
    //gui.add(&gBtnEffSwtich[14]);

    cc++;
    // 15- Noise Grain Filter Params
    gNoiseGroup.setName(ofToString(cc) + " " + noiseGrain->getName());
    gNoiseGroup.add(gBtnEffSwtich[15].getParameter());
    gNoiseGroup.add(gBtnResetEffect[15]);
    gNoiseGroup.add(gNoiseAmt.setup("Amount", 0.128, 0, 1)->getParameter());
    gNoiseGroup.add(gNoiseSpeed.setup("Speed", 0.08, 0, 1)->getParameter());
    //gui.add(&gBtnEffSwtich[15]);

    cc++;
    // 16- Tilt Shift Filter Params
    gTiltShiftGroup.setName(ofToString(cc) + " " + tiltShift->getName());
    gTiltShiftGroup.add(gBtnEffSwtich[16].getParameter());
    gTiltShiftGroup.add(gBtnResetEffect[16]);
    gTiltShiftGroup.add(gTiltFocus.setup("Focus", 0.35, 0, 1)->getParameter());
    gTiltShiftGroup.add(gTitltRange.setup("Range", 0.5, 0, 1)->getParameter());
    gTiltShiftGroup.add(gTiltOffset.setup("Offset", 0.05, 0, 1)->getParameter());
    gTiltShiftGroup.add(gTiltStrength.setup("Strength", 0.5, 0, 1)->getParameter());
    //gui.add(&gBtnEffSwtich[16]);

    cc++;
    // 17- SuperShader Filter Params
    gSupGroup.setName(ofToString(cc) + " " + superShader->getName());
    gSupGroup.add(gBtnEffSwtich[17].getParameter());
    gSupGroup.add(gBtnResetEffect[17]);
    gSupGroup.add(gSupGlowAmt.setup("Glow Amount", 0.5, 0., 1.)->getParameter());
    gSupGroup.add(gSupGlowSize.setup("Glow Size", 4.0, 0., 20.)->getParameter());
    gSupGroup.add(gSupVigOff.setup("Vignette Offset", 1.0, 0., 1.)->getParameter());
    gSupGroup.add(gSupVigDark.setup("Vignette Darkness", 1.0, 0., 1.)->getParameter());
    gSupGroup.add(gSupBri.setup("Brightness", 0.0, -1., 1.)->getParameter());
    gSupGroup.add(gSupCont.setup("Contrast", 0.0, -1., 1.)->getParameter());
    gSupGroup.add(gSupSat.setup("Saturation", 0.0, 0., 1.)->getParameter());
    gSupGroup.add(gRGBShfAmt.setup("RGB Shift Amount", 0.01, 0., 1.)->getParameter());
    //gui.add(&gBtnEffSwtich[17]);

    cc++;
    // 18- Glitch Automated Filter Params
    gGliAutoGroup.setName(ofToString(cc) + " " + glitchAuto->getName());
    gGliAutoGroup.add(gBtnEffSwtich[18].getParameter());
    gGliAutoGroup.add(gBtnResetEffect[18]);
    gGliAutoGroup.add(gGliAutoSpeed.setup("Speed", 0.6, 0 , 1)->getParameter());
    gGliAutoGroup.add(gGliAutoAmt.setup("Amount", 0.2, 0 , 1)->getParameter());
    //gui.add(&gBtnEffSwtich[18]);

    cc++;
    // 19- Space Color
    gSpaceColorGroup.setName(ofToString(cc) + " " + spaceColor->getName());
    gSpaceColorGroup.add(gBtnEffSwtich[19].getParameter());
    gSpaceColorGroup.add(gBtnResetEffect[19]);
    gSpaceColorGroup.add(gSpaceColorSpeed.setup("Speed", 1, 0, 5)->getParameter());
    gSpaceColorGroup.add(gSpaceColorOpacity.setup("Opacity", 0.1, 0, 1)->getParameter());
    //gui.add(&gBtnEffSwtich[19]);

    cc++;
    // 20- Dither
    gDitherGroup.setName(ofToString(cc) + " " + dither->getName());
    gDitherGroup.add(gBtnEffSwtich[20].getParameter());
    gDitherGroup.add(gBtnResetEffect[20]);
    gDitherGroup.add(gDitherScale.setup("Scale", 1, 0, 1)->getParameter());
    //gui.add(&gBtnEffSwtich[20]);

    cc++;
    // 21- Color Invert Strobber
    gStrobberGroup.setName(ofToString(cc) + " " + strobber->getName());
    gStrobberGroup.add(gBtnEffSwtich[21].getParameter());
    gStrobberGroup.add(gBtnResetEffect[21]);
    gStrobberGroup.add(gStrobberVolume.setup("Volume", 1, 0, 1)->getParameter());
    gStrobberGroup.add(gStrobberPhase.setup("Phase", 0.15, 0, 1)->getParameter());
    //gui.add(&gBtnEffSwtich[21]);

    cc++;
    // 22- Rimblight Pass
    gRimbLightGroup.setName(ofToString(cc) + " " + rimbShift->getName());
    gRimbLightGroup.add(gBtnEffSwtich[22].getParameter());
    gRimbLightGroup.add(gBtnResetEffect[22]);
    gRimbLightGroup.add(gRimbCol.setup("Light Color", glm::vec3(2.9, 1.3, 1.3), glm::vec3(0.0), glm::vec3(3.0))->getParameter());
    gRimbLightGroup.add(gRimbThres.setup("Intensity", 64, 0, 512)->getParameter());
    //gui.add(&gBtnEffSwtich[22]);

    cc++;
    // 23- DOF Pass
    gDofGroup.setName(ofToString(cc) + " " + dof->getName());
    gDofGroup.add(gBtnEffSwtich[23].getParameter());
    gDofGroup.add(gBtnResetEffect[23]);
    gDofGroup.add(gDofAperture.setup("Aperture", 0.8, 0.0, 1.0)->getParameter());
    gDofGroup.add(gDofFocus.setup("Focus", 0.985, 0.0, 1.0)->getParameter());
    gDofGroup.add(gDofMaxBlur.setup("Max Blur", 0.6, 0.0, 1.0)->getParameter());
    //gui.add(&gBtnEffSwtich[23]);



    // Add all Parameter groups line by line
    gui.add(gFxaaGroup);
    gui.add(gBloomGroup);
    gui.add(gKaliGroup);
    gui.add(gNoiseWrapGroup);
    gui.add(gPixelateGroup);
    gui.add(gGodRaysGroup);
    gui.add(gLimb);
    gui.add(gSsaoGroup);
    gui.add(gZoomBlurGroup);
    gui.add(gRGBGroup);
    gui.add(gFilmGrainGroup);
    gui.add(gDotScreenGroup);
    gui.add(gGlicthGroup);
    gui.add(gBadTVGroup);
    gui.add(gcolorACESGroup);
    gui.add(gNoiseGroup);
    gui.add(gTiltShiftGroup);
    gui.add(gSupGroup);
    gui.add(gGliAutoGroup);
    gui.add(gSpaceColorGroup);
    gui.add(gDitherGroup);
    gui.add(gStrobberGroup);
    gui.add(gRimbLightGroup);
    gui.add(gDofGroup);


    // 24- Fog Filter Pass
    /*gFogGroup.setName(fogFilter->getName());
    gFogGroup.add(gFogfogStart.setup("Fog Start", 0, 0, 2000)->getParameter());
    gFogGroup.add(gFogfogEnd.setup("Fog End", 10, 0, 2000)->getParameter());
    gFogGroup.add(gFogfogColor.setup("Fog Color", glm::vec4(0,0,0,1), glm::vec4(0,0,0,0), glm::vec4(1,1,1,1))->getParameter());
    gui.add(&gBtnEffSwtich[24]);
    gui.add(gFogGroup);*/

    // 26-
    gui.minimizeAll();


    // GUI LISTENERS
    gdisableAll.addListener(this, &ofxPostProcessingManager::gdisableAllHandler);
    gBypassAll.addListener(this, &ofxPostProcessingManager::gBypassAllHandler);
    eResetAll = gResetAll.newListener([this](const void * sender) {
        resetAllEffectsToDefaults();
    });
    btnLoad.addListener(this,&ofxPostProcessingManager::loadPreset);
    btnSave.addListener(this,&ofxPostProcessingManager::savePresetPressed);

    gdisableAllHandler();

    for (int i = 0; i < gui.getNumControls(); ++i) {
        //ofLogNotice(gui.getControlNames()[i]);
    }
    //ofAddListener(gui.getParameter().castGroup().parameterChangedE(), this, &ofxPostProcessingManager::paramChangedEvent);


    // GUI STYLING
    gui.getControl("SAVE PRESET")->setFillColor(ofColor(0,255,0,155)); // Check box fill color
    gui.getControl("SAVE PRESET")->setBackgroundColor(ofColor(0,0,0,150)); // bg overall
    //gui.getControl("SAVE PRESET")->setHeaderBackgroundColor(ofColor(0,0,0,150)); // Not working for ofParameter

    //ofLogNotice() << "verbose function " << gui.getGroup(dof->getName()).getFillColor();
/*    gui.getGroup(dof->getName()).setBackgroundColor(ofColor(0,200,0,150)); // bg overall
    gui.getGroup(dof->getName()).setHeaderBackgroundColor(ofColor(0,200,0,150)); // Works for ofParameterGroup
    gui.getGroup(dof->getName()).setBorderColor(ofColor(200,0,0,150));
    //gui.getGroup(dof->getName()).setFillColor(ofColor(200,0,0,150)); // Not Working
    gui.getGroup(dof->getName()).setDefaultFillColor(ofColor(200,0,0,150));*/
}

void ofxPostProcessingManager::paramChangedEvent(ofAbstractParameter &e) {
    //ofLog() << e.getName() << ": " << e.getEscapedName() << " : " << e.toString();
    //gui.getGroup()
    //ofParameter<bool> r = e.cast<bool>();
    //gui.getGroup(e.getEscapedName()).setHeaderBackgroundColor(ofColor(0,200,0,150)); // Works for ofParameterGroup
}

//---------------------------------------------------------------------------------------
int ofxPostProcessingManager::getGUIWidth() {
    return gui.getWidth();

}

glm::vec2 ofxPostProcessingManager::getGUIPos() {
    return gui.getPosition();

}
