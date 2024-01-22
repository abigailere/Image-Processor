#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
using namespace std;


struct Pixel{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

struct Header{
    vector<Pixel> pixels;
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
};

Header ReadFile(const char* filename){
   ifstream file(filename, ios_base::binary);

    Header image;
    file.read((char*)&image.idLength, sizeof (image.idLength));
    file.read((char*)&image.colorMapType, sizeof (image.colorMapType));
    file.read((char*)&image.dataTypeCode, sizeof (image.dataTypeCode));
    file.read((char*)&image.colorMapOrigin, sizeof (image.colorMapOrigin));
    file.read((char*)&image.colorMapLength, sizeof (image.colorMapLength));
    file.read((char*)&image.colorMapDepth, sizeof (image.colorMapDepth));
    file.read((char*)&image.xOrigin, sizeof (image.xOrigin));
    file.read((char*)&image.yOrigin, sizeof (image.yOrigin));
    file.read((char*)&image.width, sizeof (image.width));
    file.read((char*)&image.height, sizeof (image.height));
    file.read((char*)&image.bitsPerPixel, sizeof (image.bitsPerPixel));
    file.read((char*)&image.imageDescriptor, sizeof (image.imageDescriptor));

    int pixelCount = image.width * image.height;
    for(int i = 0; i<pixelCount; i++){
        Pixel tempPixel;
        file.read((char*)&tempPixel.blue, sizeof (tempPixel.blue));
        file.read((char*)&tempPixel.green, sizeof (tempPixel.green));
        file.read((char*)&tempPixel.red, sizeof (tempPixel.red));
        image.pixels.push_back(tempPixel);
    }
    return image;
}

void WriteFile(const char* filename, Header image){
   // Header image;
    ofstream file(filename, ios_base::binary);

    file.write(&image.idLength, sizeof (image.idLength));
    file.write(&image.colorMapType, sizeof (image.colorMapType));
    file.write(&image.dataTypeCode, sizeof (image.dataTypeCode));
    file.write((char*)&image.colorMapOrigin, sizeof (image.colorMapOrigin));
    file.write((char*)&image.colorMapLength, sizeof (image.colorMapLength));
    file.write(&image.colorMapDepth, sizeof (image.colorMapDepth));
    file.write((char*)&image.xOrigin, sizeof (image.xOrigin));
    file.write((char*)&image.yOrigin, sizeof (image.yOrigin));
    file.write((char*)&image.width, sizeof (image.width));
    file.write((char*)&image.height, sizeof (image.height));
    file.write(&image.bitsPerPixel, sizeof (image.bitsPerPixel));
    file.write(&image.imageDescriptor, sizeof (image.imageDescriptor));

    int rows = image.height;
    int cols = image.width;

    for(int i = 0; i < rows*cols; i++) {
        file.write((char*)&image.pixels[i].blue, sizeof(image.pixels[i].blue));
        file.write((char*)&image.pixels[i].green, sizeof(image.pixels[i].green));
        file.write((char*)&image.pixels[i].red, sizeof(image.pixels[i].red));
    }
    file.close();
}

void AssignImages(Header &image1, Header &image2){//assign given image header to header of new image
    image2.idLength = image1.idLength;
    image2.colorMapType = image1.colorMapType;
    image2.dataTypeCode = image1.dataTypeCode;
    image2.colorMapOrigin = image1.colorMapOrigin;
    image2.colorMapDepth = image1.colorMapDepth;
    image2.xOrigin = image1.xOrigin;
    image2.yOrigin = image1.yOrigin;
    image2.width = image1.width;
    image2.height = image1.height;
    image2.bitsPerPixel = image1.bitsPerPixel;
    image2.imageDescriptor = image1.imageDescriptor;
}

void Tests(Header &exFile, Header &testFile){
    int pixelCount = exFile.width * exFile.height;
    for(int i = 0; i<pixelCount; i++) {
        if(exFile.pixels[i].red != testFile.pixels[i].red || exFile.pixels[i].blue != testFile.pixels[i].blue || exFile.pixels[i].green != testFile.pixels[i].green) {
            cout << "index: " << i << endl;
            cout << "expected red value: " << (int)exFile.pixels[i].red << endl;
            cout << "actual red value: " << (int)testFile.pixels[i].red << endl;

            cout << "expected green value: " << (int)exFile.pixels[i].green << endl;
            cout << "actual green value: " << (int)testFile.pixels[i].green << endl;

            cout << "expected blue value: " << (int)exFile.pixels[i].blue << endl;
            cout << "actual blue value: " << (int)testFile.pixels[i].blue << endl;
        }
    }

}

int Clamp(int value){//
    //clamping
    if(value > 255){ //if reluting pixels is less than 0, then it is 0
        value = 255;
    }else if(value < 0){ //if resulting nukber is greater than 255, then its 255
        value = 0;
    }
    return value;
}

Header Multiply(Header &image1, Header &image2){
    Header resultImage;
    AssignImages(image1, resultImage);

    int pixelCount = image1.height * image1.width;
    for(unsigned int i = 0; i<pixelCount; i++){
        Pixel tempPixel;
        tempPixel.blue = (unsigned char)(((((float)image1.pixels[i].blue)/255.0f) * (((float)image2.pixels[i].blue/255))*255)+0.5f);
        tempPixel.green = (unsigned char)(((((float)image1.pixels[i].green)/255.0f) * (((float)image2.pixels[i].green/255))*255)+0.5f);
        tempPixel.red = (unsigned char)(((((float)image1.pixels[i].red)/255.0f) * (((float)image2.pixels[i].red/255))*255)+0.5f);
        resultImage.pixels.push_back(tempPixel);
    }

    return resultImage;
}

Header Screen(Header &image1, Header &image2){
    Header resultImage;
    AssignImages(image1, resultImage);
    int pixelCount = image1.width * image1.height;
    for(unsigned int i = 0; i<pixelCount; i++){
        Pixel tempPixel;
        tempPixel.blue = (((1-(1-(((float)image1.pixels[i].blue)/255.0f)) * (1-(((float)image2.pixels[i].blue)/255.0f)))*255.0f+0.5f));
        tempPixel.green = (((1-(1-(((float)image1.pixels[i].green)/255.0f)) * (1-(((float)image2.pixels[i].green)/255.0f)))*255.0f+0.5f));
        tempPixel.red = (((1-(1-(((float)image1.pixels[i].red)/255.0f)) * (1-(((float)image2.pixels[i].red)/255.0f)))*255.0f+0.5f));
        resultImage.pixels.push_back(tempPixel);
    }
    // 1-((1-NP1)*(1-NP2))

    return resultImage;
}

Header Overlay(Header &image1, Header &image2){
    Header resultImage;
    AssignImages(image1, resultImage);
    int pixelCount = image1.width*image1.height;
    for(int i=0; i<pixelCount; i++){
        //normalize all color pixels for each image
        //image 1
        float blue1 = (float) image1.pixels[i].blue/255.0f;
        float green1 = (float) image1.pixels[i].green/255.0f;
        float red1 = (float) image1.pixels[i].red/255.0f;

        //image2
        float blue2 = (float) image2.pixels[i].blue/255.0f;
        float green2 = (float) image2.pixels[i].green/255.0f;
        float red2 = (float) image2.pixels[i].red/255.0f;

        Pixel tempPixel;
        //  NP2 ≤ 0.5 :
        if((int)image2.pixels[i].blue <=0.5*255){
          //  2 · NP1 · NP2
          tempPixel.blue = Clamp((int)(((2*blue1*blue2) * 255.0f) + 0.5f));
        }else{
            //  NP2 > 0.5 : 1 − [2 · (1 − NP1) · (1 − NP2)]
            tempPixel.blue = Clamp((int)(((1-2*(1-blue1) * (1-blue2)) * 255.0f) +0.5f));
        }

        if((int)image2.pixels[i].green <=0.5*255) {
            tempPixel.green = Clamp((int) (((2 * green1 * green2) * 255.0f) + 0.5f));
        }else{
                tempPixel.green = Clamp((int)(((1-2*(1-green1) * (1-green2)) * 255.0f) +0.5f));
            }

        if((int)image2.pixels[i].red <=0.5*255) {
            tempPixel.red = Clamp((int)(((2*red1*red2) * 255.0f) + 0.5f));
        }else{
            tempPixel.red = Clamp((int)(((1-2*(1-red1) * (1-red2)) * 255.0f) +0.5f));
            }
        resultImage.pixels.push_back(tempPixel);
        }
    return resultImage;
}

Header Addition(Header &image1, Header &image2){
    Header resultImage;
    int pixelCount = image1.height * image1.width;
    AssignImages(image1, image2);

    for(unsigned int i = 0; i<pixelCount; i++){
        Pixel tempPixel;
        tempPixel.blue = (unsigned char) Clamp((int)image1.pixels[i].blue - (int)image2.pixels[i].blue);
        tempPixel.green = (unsigned char) Clamp((int)image1.pixels[i].green - (int)image2.pixels[i].green);
        tempPixel.red = (unsigned char) Clamp((int)image1.pixels[i].red - (int)image2.pixels[i].red);
        resultImage.pixels.push_back(tempPixel);
    }
    return resultImage;
}

Header Subtract(Header &image1, Header &image2){
    //top-bottom
    Header resultImage;

    AssignImages(image1, resultImage);

    int pixelCount = image1.height * image1.width;
    for(unsigned int i = 0; i<pixelCount; i++){
        Pixel tempPixel;

        tempPixel.blue = (unsigned char) (Clamp((int)image1.pixels[i].blue - (int)image2.pixels[i].blue));
        tempPixel.green = (unsigned char) (Clamp((int)image1.pixels[i].green - (int)image2.pixels[i].green));
        tempPixel.red = (unsigned char) (Clamp((int)image1.pixels[i].red - (int)image2.pixels[i].red));
        resultImage.pixels.push_back(tempPixel);
    }
    return resultImage;
}
int main(int argc, char* argv[]) {
    //task 1
    Header layer1 = ReadFile("input/layer1.tga");
    Header pattern1 = ReadFile("input/pattern1.tga");
    Header result1 = Multiply(layer1, pattern1);
    WriteFile("output/part1.tga", result1);

    //task 2
    Header layer2 = ReadFile("input/layer2.tga");
    Header car = ReadFile("input/car.tga");
    Header result2 =  Subtract(car, layer2);
    WriteFile("output/part2.tga", result2);


    //task 3
    Header pattern2 = ReadFile("input/pattern2.tga");
    Header tempResult = Multiply(pattern2, layer1);
    Header text = ReadFile("input/text.tga");
    Header result3 = Screen(text,tempResult);
    WriteFile("output/part3.tga", result3);

    //task4
    Header circles = ReadFile("input/circles.tga");
    Header tempResult2 = Multiply(layer2, circles);
    //Header pattern2 = ReadFile("input/pattern2.tga");
    Header result4 = Subtract(tempResult2, pattern2);
    WriteFile("output/part4.tga", result4);

    //task 5
    Header result5;
    AssignImages(layer1, result5);
    result5 = Overlay(layer1, pattern1);
    WriteFile("output/part5.tga", result5);

    //task 6 done
    int pixelCount = car.width * car.height;
    for(unsigned int i = 0; i<pixelCount; i++){
        car.pixels[i].green =  Clamp(car.pixels[i].green + 200);
    }
    Header result6 = car;
    WriteFile("output/part6.tga", result6);

    //task 7
    Header result7;
    AssignImages(car,result7);
    for(unsigned int i = 0; i<pixelCount; i++){
        Pixel tempPixel;
        tempPixel.blue = ((int)car.pixels[i].blue) * 0;
        tempPixel.green = ((int)car.pixels[i].green);
        tempPixel.red = ((int)car.pixels[i].red) * 4;
        result7.pixels.push_back(tempPixel);
    }

    WriteFile("output/part7.tga", result7);

    //task 8
    Header result8red;
    Header result8blue;
    Header result8green;
    AssignImages(car, result8blue);
    AssignImages(car, result8red);
    AssignImages(car, result8green);
    int pixelCount3 = car.height * car.width;
    for(int i = 0; i<pixelCount3; i++){
        Pixel tempPixelRed;
        Pixel tempPixelBlue;
        Pixel tempPixelGreen;
        tempPixelBlue.blue = car.pixels[i].blue;
        tempPixelBlue.green = car.pixels[i].blue;
        tempPixelBlue.red = car.pixels[i].blue;
        result8blue.pixels.push_back(tempPixelBlue);

        tempPixelGreen.blue = car.pixels[i].green;
        tempPixelGreen.green = car.pixels[i].green;
        tempPixelGreen.red = car.pixels[i].green;
        result8green.pixels.push_back(tempPixelGreen);

        tempPixelRed.blue = car.pixels[i].red;
        tempPixelRed.green = car.pixels[i].red;
        tempPixelRed.red = car.pixels[i].red;
        result8red.pixels.push_back(tempPixelRed);
    }
    WriteFile("output/part8_b.tga", result8blue); //not same
    WriteFile("output/part8_g.tga", result8green); //not same
    WriteFile("output/part8_r.tga", result8red); //not same

    //task 9 not same
    Header result9;
    Header layer_red = ReadFile("input/layer_red.tga");
    Header layer_green = ReadFile("input/layer_green.tga");
    Header layer_blue = ReadFile("input/layer_blue.tga");
    AssignImages(layer_red, result9);

    int pixelCount2 = layer_red.height * layer_red.width;
    for(unsigned int i=0; i<pixelCount2; i++){
        Pixel tempPixel;
        tempPixel.blue = layer_blue.pixels[i].blue;
        tempPixel.green = layer_green.pixels[i].green;
        tempPixel.red= layer_red.pixels[i].red;
        result9.pixels.push_back(tempPixel);
    }
    WriteFile("output/part9.tga", result9);

    //task 10
    Header text2 = ReadFile("input/text2.tga");
    Header result10;
    AssignImages(text2, result10);
    for(int i = text2.pixels.size()-1; i>=0; i--){
        Pixel tempPixel;
        tempPixel.blue = text2.pixels[i].blue;
        tempPixel.green = text2.pixels[i].green;
        tempPixel.red = text2.pixels[i].red;
        result10.pixels.push_back(tempPixel);
    }
    WriteFile("output/part10.tga", result10);


    return 0;
}



