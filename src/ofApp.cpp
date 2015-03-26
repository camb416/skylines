#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
// image
    img.loadImage("gugg.jpg");


    img.resize(320,240);
    colorImg.allocate(320,240);
    grayImage.allocate(320,240);
    grayBg.allocate(320,240);
    grayDiff.allocate(320,240);
    
    bLearnBakground = true;
    threshold = 128;
    
    rotDiff.allocate(320,320);
    


}

//--------------------------------------------------------------
void ofApp::update(){
  

    
    colorImg.setFromPixels(img.getPixels(), 320,240);

//    colorImg.convertToGrayscalePlanarImages(redChan, greenChan,blueChan);
    colorImg.convertToGrayscalePlanarImage(redChan, 0);
    colorImg.convertToGrayscalePlanarImage(greenChan, 1);
    colorImg.convertToGrayscalePlanarImage(blueChan, 2);
    
    grayImage = colorImg;
    
    if (bLearnBakground == true){
        grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
        bLearnBakground = false;
    }
    

    grayDiff = redChan;
    //grayDiff.absDiff(grayBg, grayImage);

    grayDiff.threshold(threshold); // grayDiff the first ofxCvGrayscaleImage (320x240)
    rotDiff.setROI(0,0,320,240); // rotDiff is the second (320x320)
    rotDiff = grayDiff;
    rotDiff.resetROI();
    rotDiff.rotate(ofGetFrameNum(),160,160);
    
    // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    // also, find holes is set to true so we will get interior contours as well....
    contourFinder.findContours(grayDiff, 20, (340*240)/3*2, 10, true);	// find holes
}

//--------------------------------------------------------------
void ofApp::draw(){
    //img.draw(0,0,320,240);
    
    // draw the incoming, the grayscale, the bg and the thresholded difference
    ofSetHexColor(0xffffff);
    colorImg.draw(20,20);
    grayImage.draw(20,20+10+10+75+240);
    ofSetColor(255,0,0);
    redChan.draw(20,30+240*1,100,75);
    ofSetColor(0,255,0);
    greenChan.draw(20+10+100,30+240*1,100,75);
    ofSetColor(0,0,255);
    blueChan.draw(20+20+200,30+240*1,100,75);
    
    ofSetColor(255,255,255);
    //grayBg.draw(20,280);
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    grayDiff.draw(20,240+10+10+20+75);
    ofDisableBlendMode();
    
    rotDiff.draw(20+320+10,20+240+10);
    
    
    ofFill();
    ofSetHexColor(0x333333);
    ofRect(320+20+10,20,320,240);
    ofSetHexColor(0xffffff);
    
    // we could draw the whole contour finder
    //contourFinder.draw(360,540);
    
    // or, instead we can draw each blob individually from the blobs vector,
    // this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(320+20+10,20);
        
        // draw over the centroid if the blob is a hole
        ofSetColor(255);
        if(contourFinder.blobs[i].hole){
            ofDrawBitmapString("x",
                               contourFinder.blobs[i].boundingRect.getCenter().x + 320+20+10,
                               contourFinder.blobs[i].boundingRect.getCenter().y + 20);
        }
    }
    
    // finally, a report:
    ofSetHexColor(0xffffff);
    stringstream reportStr;
    reportStr << "press ' ' to capture bg" << endl
    << "threshold " << threshold << " (press: +/-)" << endl
    << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
    ofDrawBitmapString(reportStr.str(), 20, 240+240+75+20+10+10+10+10);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
        case ' ':
            bLearnBakground = true;
            break;
        case '+':
            threshold ++;
            if (threshold > 255) threshold = 255;
            break;
        case '-':
            threshold --;
            if (threshold < 0) threshold = 0;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
