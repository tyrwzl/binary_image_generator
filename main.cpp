#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

std::vector<std::string> loadImages(const std::string& path)
{
    std::vector<std::string> filenames;

    fs::path targetDir(path);
    fs::directory_iterator eod;

    for (fs::directory_iterator it(targetDir); it != eod; it++) {
        const fs::path p = *it;
        if (fs::is_regular_file(p)) {
            std::string filename = p.native();
            if (filename.find("_oracle.png") != std::string::npos) {
                size_t pos = filename.find("_oracle.png");
                filename.replace(pos, filename.length(), "");
                filenames.push_back(filename);
            }

        } else {
            std::cout << "ERROR" << std::endl;
        }
    }

    std::sort(filenames.begin(), filenames.end());

    return filenames;
}


int main(int argc, char** argv)
{
    /*
    cv::Mat image = cv::imread("20160221_043318_00/1.bmp");
    cv::namedWindow("image");
    cv::imshow("image", image);
    cv::waitKey(0);
    */

    std::string path = "20160221_043318_00";

    std::vector<std::string> images = loadImages(path);

    for (std::string filename : images) {
        std::ifstream ifs(filename.c_str(), std::ifstream::binary);
        if (!ifs.is_open()) {
            return false;
        }

        int rows, cols, type;
        ifs.read((char*)(&rows), sizeof(int));
        if (rows == 0) {
            return true;
        }

        ifs.read((char*)(&cols), sizeof(int));
        ifs.read((char*)(&type), sizeof(int));

        cv::Mat depthData;
        depthData.create(rows, cols, type);
        ifs.read((char*)(depthData.data), depthData.elemSize() * depthData.total());

        cv::imwrite(filename+".png", depthData);
    }
    return 0;
}
