//
//  ofxImgWarp.cpp
//  mlsExample
//
//  Created by Roee Kremer on 7/15/13.
//
//

#include "ofxImgWarp.h"

/*
 double calcArea(const vector< cv::Point_<double> > &V){
 cv::Point_<double> lt, rb;
 lt.x = lt.y = 1e10;
 rb.x = rb.y = -1e10;
 for (vector< cv::Point_<double> >::const_iterator i = V.begin();
 i!=V.end(); i++){
 if (i->x < lt.x) lt.x = i->x;
 if (i->x > rb.x) rb.x = i->x;
 if (i->y < lt.y) lt.y = i->y;
 if (i->y > rb.y) rb.y = i->y;
 }
 return (rb.x-lt.x)*(rb.y-lt.y);
 }
 */

void ofxImgWarp::setup(int width,int height,int gridSize) {
    this->gridSize = gridSize;
    rDx.allocate(width, height, 1);
    rDy.allocate(width, height, 1);
}

void ofxImgWarp::calcDelta(vector<pair<ofPoint,ofPoint> > &ctrl){
    int i, j, k;
    
    ofPoint swq, qstar, newP, tmpP;
    float sw;
    
    
    int tarW = rDx.getWidth();
    int tarH = rDx.getHeight();
    
    int nPoint = ctrl.size();
    
    //    if (preScale){
    //        ratio = sqrt(calcArea(newDotL) / calcArea(oldDotL));
    //        for (i=0; i<nPoint; i++)
    //            newDotL[i] *= 1/ratio;
    //    }
    
    
    
    float *w=new float[nPoint];
    
    
    
    
	if (nPoint < 2){
		rDx.set(0);
		rDy.set(0);
		return;
	}
    /*
     rDx.create(tarH, tarW);
     rDy.create(tarH, tarW);
     
     if (nPoint < 2){
     rDx.setTo(0);
     rDy.setTo(0);
     return;
     }
     */
    ofPoint swp, pstar, curV, curVJ, Pi, PiJ, Qi;
    float miu_r;
    
    for (i = 0; ; i+=gridSize){
        if (i>=tarW && i<tarW+gridSize - 1)
            i=tarW-1;
        else if (i>=tarW)
            break;
        for (j = 0; ; j+=gridSize){
            if (j>=tarH && j<tarH+gridSize - 1)
                j = tarH - 1;
            else if (j>=tarH)
                break;
            sw = 0;
            swp.x = swp.y = 0;
            swq.x = swq.y = 0;
            newP.x = newP.y = 0;
            curV.x = i;
            curV.y = j;
            for (k = 0; k < nPoint; k++){
                if ((i==ctrl[k].second.x) && j==ctrl[k].second.y)
                    break;
                //                if (alpha==1)
                w[k] = 1/((i-ctrl[k].second.x)*(i-ctrl[k].second.x)+
                          (j-ctrl[k].second.y)*(j-ctrl[k].second.y));
                //                else
                //                    w[k] = pow((i-ctrl[k].second.x)*(i-ctrl[k].second.x)+
                //                               (j-ctrl[k].second.y)*(j-ctrl[k].second.y), -alpha);
                sw = sw + w[k];
                swp = swp + w[k] * ctrl[k].second;
                swq = swq + w[k] * ctrl[k].first;
            }
            if ( k == nPoint ) {
                pstar = (1 / sw) * swp ;
                qstar = 1/sw * swq;
                //            qDebug("pstar: (%f, %f)", pstar[0], pstar[1]);
                
                // Calc miu_r
                //miu_s = 0;
                double s1=0, s2=0;
                for (k = 0; k < nPoint; k++){
                    if (i==ctrl[k].second.x && j==ctrl[k].second.y)
                        continue;
                    
                    Pi = ctrl[k].second - pstar;
                    PiJ.x = -Pi.y, PiJ.y = Pi.x;
                    Qi = ctrl[k].first - qstar;
                    s1 += w[k] * Qi.dot(Pi);
                    s2 += w[k] * Qi.dot(PiJ);
                }
                miu_r = sqrt(s1*s1 + s2*s2);
                
                curV -= pstar;
                curVJ.x = -curV.y, curVJ.y = curV.x;
                
                for (k = 0; k < nPoint; k++){
                    if (i==ctrl[k].second.x && j==ctrl[k].second.y)
                        continue;
                    
                    Pi = ctrl[k].second - pstar;
                    PiJ.x = -Pi.y, PiJ.y = Pi.x;
                    
                    tmpP.x = Pi.dot(curV) * ctrl[k].first.x
                    - PiJ.dot(curV) * ctrl[k].first.y;
                    tmpP.y = -Pi.dot(curVJ) * ctrl[k].first.x
                    + PiJ.dot(curVJ) * ctrl[k].first.y;
                    tmpP *= w[k]/miu_r;
                    newP += tmpP;
                }
                newP += qstar;
            }
            else {
                newP = ctrl[k].first;
            }
            
            //            if (preScale){
            //                rDx(j, i) = newP.x * ratio - i;
            //                rDy(j, i) = newP.y * ratio - j;
            //            }
            //            else {
            //                printf("%i\t%i\t%f\t%f\n",i,j,newP.x,newP.y);
            //                cout << i << '\t' << j << '\t' << newP.x << '\t' << newP.y << endl;
            rDx[rDx.getPixelIndex(i, j)] = newP.x - i;
            rDy[rDy.getPixelIndex(i, j)] = newP.y - j;
            
            //                rDx(j, i) = newP.x - i;
            //                rDy(j, i) = newP.y - j;
            //            }
        }
    }
    delete [] w;
    
    //    if (preScale){
    //        for (i=0; i<nPoint; i++)
    //            newDotL[i] *= ratio;
    //    }
    
    //    qDebug("Calc OK");
    // cout<<rDx<<endl;
}


inline float bilinear_interp(float x, float y,
                             float v11, float v12, float v21, float v22){
    return (v11*(1-y) + v12*y) * (1-x) + (v21*(1-y) + v22*y) * x;
}


void ofxImgWarp::genNewImg(ofPixels &src,ofPixels &dst, float transRatio){
    int i,j;
    float di, dj;
    float nx, ny;
    int nxi, nyi, nxi1, nyi1;
    float deltaX, deltaY;
    double w, h;
    int ni, nj;
    
    
    int srcW = src.getWidth();
    int srcH = src.getHeight();
    int tarW = dst.getWidth();
    int tarH = dst.getHeight();
    
    //    Mat newImg(tarH, tarW, oriImg.type());
    for (i=0;i<tarH;i+=gridSize)
        for (j=0;j<tarW;j+=gridSize){
            ni = i+gridSize;
            nj = j+gridSize;
            w = h = gridSize;
            if (ni>=tarH) ni = tarH-1, h = ni-i+1;
            if (nj>=tarW) nj = tarW-1, w = nj-j+1;
            for (di=0; di<h; di++)
                for (dj=0; dj<w; dj++){
                    //                    deltaX = bilinear_interp(di/h, dj/w, rDx(i,j),rDx(i, nj), rDx(ni, j), rDx(ni, nj));
                    deltaX = bilinear_interp(di/h, dj/w, rDx[rDx.getPixelIndex(j, i)],rDx[rDx.getPixelIndex(nj, i)], rDx[rDx.getPixelIndex(j, ni)], rDx[rDx.getPixelIndex(nj, ni)]);
                    //                    qDebug("(%d, %d, %d, %d): (%f, %f, %f, %f)",i, j, ni, nj, rDx(i,j),rDx(i, nj), rDx(ni, j), rDx(ni, nj));
                    //                    deltaY = bilinear_interp(di/h, dj/w, rDy(i,j),rDy(i, nj), rDy(ni, j), rDy(ni, nj));
                    deltaY = bilinear_interp(di/h, dj/w, rDy[rDy.getPixelIndex(j, i)],rDy[rDy.getPixelIndex(nj, i)], rDy[rDy.getPixelIndex(j, ni)], rDy[rDy.getPixelIndex(nj, ni)]);
                    //                  cout << deltaX << '\t' << deltaY << endl;
                    nx = j + dj + deltaX * transRatio;
                    ny = i + di + deltaY * transRatio;
                    //                     printf("%dx%d\n", srcW, srcH);
                    if (nx>srcW-1) nx = srcW-1;
                    if (ny>srcH-1) ny = srcH-1;
                    //                     if (nx>srcH-1) nx = srcH-1;
                    //                     if (ny>srcW-1) ny = srcW-1;
                    if (nx<0) nx = 0;
                    if (ny<0) ny = 0;
                    nxi = int(nx);
                    nyi = int(ny);
                    nxi1 = ceil(nx);
                    nyi1 = ceil(ny);
                    
                    //                    if (oriImg.channels() == 1)
                    //                        newImg.at<uchar>(i+di, j+dj) =
                    //                        bilinear_interp(ny-nyi, nx-nxi,oriImg.at<uchar>(nyi, nxi), oriImg.at<uchar>(nyi, nxi1),oriImg.at<uchar>(nyi1, nxi), oriImg.at<uchar>(nyi1, nxi1));
                    //                    else {
                    //                        for (int ll=0; ll<3; ll++)
                    //                            newImg.at<Vec3b>(i+di, j+dj)[ll] =
                    //                            bilinear_interp(ny-nyi, nx-nxi,oriImg.at<Vec3b>(nyi, nxi)[ll], oriImg.at<Vec3b>(nyi, nxi1)[ll], oriImg.at<Vec3b>(nyi1, nxi)[ll], oriImg.at<Vec3b>(nyi1, nxi1)[ll]);
                    //                    }
                    
                    
                    
                    for (int ll=0; ll<src.getNumChannels(); ll++) {
                        
                        //                        cout << nxi << '\t' << nyi << endl;
                        dst[dst.getPixelIndex(j+dj, i+di)+ll]=bilinear_interp(ny-nyi, nx-nxi,
                                                                              src[src.getPixelIndex(nxi, nyi)+ll],
                                                                              src[src.getPixelIndex(nxi1, nyi)+ll],
                                                                              src[src.getPixelIndex(nxi, nyi1)+ll],
                                                                              src[src.getPixelIndex(nxi1, nyi1)+ll]);
                    }
                    
                    
                }
        }
    //    return newImg;
    
}
