/**
* This file is part of ORB-SLAM3
*
* Copyright (C) 2017-2020 Carlos Campos, Richard Elvira, Juan J. Gómez Rodríguez, José M.M. Montiel and Juan D. Tardós, University of Zaragoza.
* Copyright (C) 2014-2016 Raúl Mur-Artal, José M.M. Montiel and Juan D. Tardós, University of Zaragoza.
*
* ORB-SLAM3 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
* License as published by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM3 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
* the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with ORB-SLAM3.
* If not, see <http://www.gnu.org/licenses/>.
*/



#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<System.h>

using namespace std;
using namespace cv;

void LoadImages(const string &strImagePath, const string &strPathTimes,
                vector<string> &vstrImages, vector<double> &vTimeStamps);

int main(int argc, char **argv)
{  
    if(argc != 5)
    {
        cerr << endl << "Usage: ./mono_euroc path_to_vocabulary path_to_settings path_to_video trajectory_file_name" << endl;
        return 1;
    }

    VideoCapture capture(argv[3]);
    int num_frames = capture.get(CAP_PROP_FRAME_COUNT);
    double fps = capture.get(CAP_PROP_FPS);
    cout << "Number of frames: " << num_frames << endl;
    cout << "FPS: " << fps << endl;


    // Vector for tracking time statistics
    vector<float> vTimesTrack;
    vTimesTrack.resize(num_frames);

    cout << endl << "-------" << endl;
    cout.precision(17);

    // Create SLAM system. It initializes all system threads and gets ready to process frames.

    ORB_SLAM3::System SLAM(argv[1],argv[2],ORB_SLAM3::System::MONOCULAR, false);

    if (!capture.isOpened()) {
        cerr << endl << "Error opening video " << argv[3] << endl;
        return 1;
    }

    Mat im;
    int ind = 0;
    double tframe = 0;

    while (1) {
        capture >> im;
        if (im.empty()) {
            cout << "End of video" << endl;
            break;
        }
        ind += 1;
        cv::resize(im, im, cv::Size(960, 540));

        if (ind % 90 == 0) {
            cout << ind << "/" << num_frames << " frames" << endl;
        }

#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t1 = std::chrono::monotonic_clock::now();
#endif

        // Pass the image to the SLAM system
        SLAM.TrackMonocular(im, tframe);

#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t2 = std::chrono::monotonic_clock::now();
#endif

#ifdef REGISTER_TIMES
        double t_track = std::chrono::duration_cast<std::chrono::duration<double,std::milli> >(t2 - t1).count();
        SLAM.InsertTrackTime(t_track);
#endif

        double ttrack= std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count();

        vTimesTrack[ind]=ttrack;

        tframe += 1.0 / fps;
        ind += 1;

        // cv::imshow("video", im);
        // cv::waitKey(1);

    }

    // Stop all threads
    SLAM.Shutdown();

    // Save camera trajectory

    const string kf_file =  "kf_" + string(argv[argc-1]) + ".txt";
    const string f_file =  "f_" + string(argv[argc-1]) + ".txt";
    SLAM.SaveTrajectoryEuRoC(f_file);
    SLAM.SaveKeyFrameTrajectoryEuRoC(kf_file);
    

    return 0;
}