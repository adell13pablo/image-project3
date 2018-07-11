#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
//#pragma GCC poison texture_image
#pragma GCC diagnostic ignored "-Wwrite-strings"
//OpenCV libraries
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>


#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#define  BLOCK_SIZE 4
#include <stdio.h>
#include <iostream>
// Image Type
#include <image.h>
#include <cmath> //Library for math functions
// GUI
#include <imgui/imgui.h>
#include <imgui/local.h>  // reset_image, texture_image,
#include <imgui/imgui_glfw.h>



// STL
#include <vector>
#include <string>

// Local functions
static ImGuiWindowFlags static_window();
static void error_callback(int error, const char* description);

using namespace cv;

/*
Author: Pablo Adell
Date: 01-24-2018
*/



void resize_image(Image* img, int new_w, int new_h)

//Code Provided by scarface382 --> https://github.com/scarface382/multimedia-examples/blob/master/8%20-%20Image%20to%20image%20embedding/main.cpp
{
    Image new_image;
    uint8* resized_img_pixels = (uint8*)malloc(new_w * new_h * RGBA); //allocates new memory for new_w x new_h multiplied byt RGB

    float w_ratio = (float)new_w / img->width; // ratio dictates how many pixels to copy from original image per pixel in width
    float h_ratio = (float)new_h / img->height; // ratio dictates how many pixels to copy from original image per pixel in height

    int max_x_repeat = w_ratio <= 1.0 ? 1 : w_ratio + 1.0; // if ratio is 2.3, then round up to 3, hence + 1
    int max_y_repeat = h_ratio <= 1.0 ? 1 : h_ratio + 1.0;
    //if the ratio is less than or equal to one, then the max number of times the pixel can be repeated is once`,
    //else it can be repeated more than once, specifically determined by the ratio
    int c = 0;

    for(int y=0; y < img->height; y++) {
        uint8* row = img->pixels + (y * img->width * RGBA); //itterates through original image row

        for(int x=0; x < img->width; x++) {
            uint8* pixel = row + (x * RGBA); //itterates through original image pixels

            for(int y_repeat=0; y_repeat < max_y_repeat; y_repeat++) {

                int length_y = (int)(y * h_ratio) + y_repeat;
                if (length_y >= new_h) // if image insert copying is out of bounds from image frame in relation to height, breaks
                    break;

                uint8* row_new_image = resized_img_pixels + (((int)(y * h_ratio) + y_repeat) * new_w * RGBA);// keeps track of orignal row transfer to new image

                for(int x_repeat=0; x_repeat < max_x_repeat; x_repeat++) {

                    int length_x = (int)(x * w_ratio) + x_repeat;
                    if (length_x >= new_w) // if image insert copying is out of bounds from image frame in relation to width, breaks
                        break;

                    uint8* pixel_new_image = row_new_image + (((int)(x * w_ratio) + x_repeat) * RGBA); //keeps track of what original pixels are copied to new image

                    //if enlarging, can repeat the copying of pixels
                    pixel_new_image[0] = pixel[0]; //R
                    pixel_new_image[1] = pixel[1]; //G
                    pixel_new_image[2] = pixel[2]; //B
                    pixel_new_image[3] = pixel[3]; //A
                    resized_img_pixels[c] =       pixel_new_image[0];
                    resized_img_pixels[c+1] =       pixel_new_image[1];
                    resized_img_pixels[c+2] =       pixel_new_image[2];
                    resized_img_pixels[c+3] =       pixel_new_image[3];
                  //  std::cout << "pixel is " <<  (int)pixel[149];
                    c=+4;

                }
            }
        }
    }
    new_image.width = new_w;
    new_image.height = new_h;
    new_image.pixels = resized_img_pixels;
    texture_image(&new_image);
    *img = new_image;
  }

ImVec2 resize(int x, int y)
{
    if (x > 200 && y > 200) {
        float d = (x > y ? x : y) / 200;
        return ImVec2( x/d, y/d );
    }
    return ImVec2(x, y);
}

void capture_video( String filename) {
  //Method to capture video from a file
  VideoCapture capture(filename); //Get the video
  Mat frame; //This variable will store every frame captured on the Video

  if(!capture.isOpened()){
    throw "Error opening the file";
  }

    namedWindow("w", 1); //open OpenCV window
    for( ; ; ){
      capture >> frame;
      //Call method on frame
      if(frame.empty()){
        break; //Stop capturing video once it is done
      }
      imshow("w", frame); //Use openCV window to show the frame
      waitKey(20); //Wait til next frame
    }
    waitKey(0); //Key press to close window releasing and destroying the window
}



void mask_face(Image a, Image b,  uint8* pixels, Mat original, Mat mask){
  if(original.empty()) printf("Error abriendo el archivo\n");
  printf("Testing OpenCV\n");
  printf("Loading Haar-Cascade\n");
  CascadeClassifier faceDetector;
  bool loaded1 = faceDetector.load("data/haarcascade_frontalface_default.xml");
  printf("Training Loaded? %d\n", loaded1 );
//  b = resize_image(b, 100, 100);

  std::cout << "\nB width and height "<< b.width << " , " << b.height ;

  std::vector<Rect> faces; //This will store the values where the faces are at (x,y,w,h), as well as the size of them
//  cvtColor(original, original, COLOR_BGR2GRAY); //Change color of image from rgb to greyscale
//  equalizeHist(original, original); //Equialize the histogram of the image so the contrast is deeper and we can detect better the faces

//faceDetector.detectMultiScale(original, faces, 1.30, 2, 0|CASCADE_SCALE_IMAGE, Size(150, 150)); //Detect the faces from src, storing it into a vector with max size
faceDetector.detectMultiScale(original, faces,1.1, 2, 0 | 1, Size(40, 40), Size(400,400 ));
  std::cout << "\nFaces detected " << faces.size();
  for(int i = 0; i< faces.size(); i++){
    std::cout << "\nFace " << i << faces[i];
    std::cout << "\nFace_X " << i << faces[i].x;
      std::cout << "\nFace_y " << i << faces[0].y;
  }
  //Once we have the faces, iterate through them

  printf("\nA data are: width %d, height %d, total pixels %d\n", a.width, a.height, (a.width * a.height) );
  int len = a.width * a.height * 4;
int j = 0;
int k = 0;
Image new_image;
std::cout << "\nOriginal cols and rows: " << original.cols  << " , "<< original.rows;
new_image.width = (int)original.cols;
new_image.height = (int) original.rows;
new_image.pixels = (uint8*)malloc(new_image.width*new_image.height*4);
std::cout << "\nNew Images cols and rows: " << new_image.width  << " , "<< new_image.height;
std::cout << " \nA length: " << len ;
std::cout << "\n New image length: " <<  new_image.height*new_image.width*4;
uint8_t* pixelPtr = (uint8_t*)original.data;
uint8_t* newImagepixel = (uint8_t*)new_image.pixels;



/*for(int g = 0; g<( new_image.height*new_image.width* 3); g = g+3 ){
  new_image.pixels[g+2] = original.data[g]; //B
  //std::cout << "\nNEW IMAGE Original " <<  (int) original.data[g];
//  std::cout << "\nNEW IMAGE " <<  (int) new_image.pixels[g]


*newImagepixel =  pixelPtr[g+2]; //R
newImagepixel++ ;
*newImagepixel =  pixelPtr[g+1]; //G
newImagepixel++ ;
*newImagepixel =  pixelPtr[g]; //B
newImagepixel++ ;
*newImagepixel = 255; //A
newImagepixel++ ;



//  std::cout << "\nG " <<  g;
}*/

  for(int i =0; i<len; i++)
{
    pixels[i] = a.pixels[i];
  }



  std::cout<< "CHANNELS " << original.channels();


//int final_size = (faces[0].width < faces[1].width ? faces[0].width : faces[1].width);
int final_size = (faces[0].width < b.width ? faces[0].width : b.width);
resize_image(&b, faces[0].width, faces[0].width );

std::cout << "HASTA AQUI LLEGO";
std::cout << "New b data are : " << b.width << " , " << b.height << " PIXEL 150 is: " << (int) b.pixels[200] ;

j= 0;

printf("Final size %d\n", final_size * final_size );


  for(int x = faces[0].y; x < faces[0].y + b.height; x++){
      for(int y = faces[0].x*4 ; y < (faces[0].x + b.width)*4 ; y++){


              if(b.pixels[j] ==0){
            

                pixels[(a.width * 4 * x) + y] = pixels[(a.width * 4 * x) + y] ;
              }else{
             pixels[(a.width * 4 * x) + y] =(b.pixels[j]);
}






      j++;


    }

  }

  std::cout << "\n J is equal: " << j;

j = 0;

}
  //Hace sto para que avance de 4 en 4 y meter todos los valores en un array para comparar lo con las muestras caucasicas que hemos cogido antes --> (255,224, 189) (255,205,148) (234, 192, 134) (255,173, 96) (255, 227 159)



//pops up the file list
std::string image_select(int s)
{
    static std::vector<std::string> selected = {"",""};
    ImGui::SameLine();
    ImGui::Text(selected[s] == "" ? "<None>" : selected[s].c_str());

    std::string current_dir;


    int sindex;
    if  (s == 0)
        sindex = fs(selected[s], current_dir, "select 0");

    if  (s == 1)
        sindex = fs(selected[s], current_dir, "select 1");



    if (sindex == -1 )
        return "";
    else
        return current_dir + "/" + selected[s];
}












int main()
{
    Image a;
    Image b;
    Image c;
    Mat original;
    Mat mask;
    std::vector<std::string> fnames = {"",""};
    std::string bits_name = "1";

    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1200,450, "Project 1", NULL, NULL);
    // glfwSetWindowPos(window, -1500, 100);
    glfwMakeContextCurrent(window);

    glewInit();

    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

    bool slider_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
      glfwPollEvents();
      ImGui_ImplGlfwGL3_NewFrame();

      ImGui::SetNextWindowSize(ImVec2(600,200), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("My project");

      {
        // Image A
        ImGui::BeginGroup();
        {
          if (a.texture_loaded){

                ImGui::Image((void*)a.texture, resize(a.width, a.height));
          }
            if (ImGui::Button("Start"))
                ImGui::OpenPopup("select 0");
            std::string tmp = image_select(0); //opens the file list and assigns to tmp
            if (tmp != "") {
                fnames[0] = tmp;
                reset_image(&a);
                reset_image(&b);
                //Method to capture video from a file
               a.pixels = stbi_load(fnames[0].c_str(), &a.width, &a.height, &a.n, RGBA);
                original = imread(fnames[0].c_str(), IMREAD_COLOR); //Creating Mat object over which the mask will be applied
                texture_image(&a);
              }

            if(ImGui::Button("Reset")){
                reset_image(&a);
                reset_image(&b);
                  reset_image(&c);
                free_image(&a);
                free(a.pixels);
                free_image(&b);
                free(b.pixels);
                free_image(&c);
                free(c.pixels);
            }
        }

           ImGui::EndGroup();
           ImGui::SameLine();

               ImGui::BeginGroup();
              {

                   if(b.texture_loaded){

                   ImGui::Image((void*)b.texture, resize(b.width, b.height));

                   }

                   if(ImGui::Button("Select mask"))
                       ImGui::OpenPopup("select 1");


                   std::string tmp_2 = image_select(1);
                   if(tmp_2 !=""){
                   fnames[0] = tmp_2;
                   reset_image(&b);
                   reset_image(&c);

                   b.pixels = stbi_load(fnames[0].c_str(), &b.width, &b.height, &b.n, RGBA);
                   texture_image(&b);
                 /*  if((b.width * b.height) > 19200){
                   b = resize_4(b);
                   texture_image(&b);
                 }*/
                   }

              }
               ImGui::EndGroup();

           ImGui::SameLine();

            ImGui::BeginGroup();
           {


                if(a.texture_loaded && b.texture_loaded &&!c.texture_loaded){
                if(ImGui::Button("Apply mask")){
                  c.width = a.width;
                  c.height = a.height;
                  c.pixels = (uint8*)malloc(c.width * c.height * RGBA);
                  mask_face(a, b, c.pixels, original, mask);
                  texture_image(&c);
                  bits_name = "mask";

                }
              }
                if(c.texture_loaded){
                  ImGui::Image((void*)c.texture, resize(c.width, c.height));

                                  if (ImGui::Button("Save")) { //When clicked it saves the picture as bit_des.png
                                    char sname[1000];
                                    bits_name.append("_face_mask.png");
                                    sprintf(sname, bits_name.c_str());
                                    stbi_write_png(sname, c.width, c.height, 4, c.pixels, 0);
                                  }
                }



                }



            ImGui::EndGroup();
           //Create Image when process is clicked

            ImGui::SameLine();




        }




      ImGui::End();

      // Gl Rendering
      int display_w, display_h;
      glfwGetFramebufferSize(window, &display_w, &display_h);
      glViewport(0, 0, display_w, display_h);
      glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui::Render();
      glfwSwapBuffers(window);

    }  //end of while

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();
    free_image(&a);
    free_image(&b);
    free(a.pixels);
    free(b.pixels);

    return 0;
} //end of main

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

ImGuiWindowFlags static_window()
{
    static bool no_titlebar = false;
    static bool no_border = true;
    static bool no_resize = false;
    static bool no_move = false;
    static bool no_scrollbar = false;
    static bool no_collapse = false;
    static bool no_menu = true;

    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
    if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
    if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
    if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
    if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;
    return window_flags;
}

/*
       ImGui::BeginGroup();
           {
                if(b.texture_loaded){
                    ImGui::Image((void*)b.texture, resize(b.width, b.height));
                }

                if(ImGui::Button("Select watermark"))
                    ImGui::OpenPopup("select 0");


                std::string tmp_2 = image_select(1);
                if(tmp_2 !=""){
                fnames[0] = tmp_2;
                reset_image(&b);
                reset_image(&c);

                b.pixels = stbi_load(fnames[0].c_str(), &b.width, &b.height, &b.n, RGBA);
                texture_image(&b);
                }

           }
            ImGui::EndGroup();
           //Create Image when process is clicked
        ImGui::BeginGroup();{


               if (a.texture_loaded && b.texture_loaded && !c.texture_loaded) { //If a is loaded but b is not
                c.width = a.width;
                c.height = a.height;
                c.pixels = (uint8*)malloc(a.width * a.height * RGBA); //Allocate the pixels for b
               }

            if(c.texture_loaded != 1){
                if (c.pixels && ImGui::Button("Process jump 1 bit!")) { //If the pixels have been allocated, create the image and point to its address for the texture

                texture_image(&c);
                bits_name = "1";
                                    }


             }

            if (c.texture_loaded) { //If b has been created, show it on the display
            ImGui::Image((void*)b.texture, resize(c.width, c.height), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(0,255,0,255));
            if (ImGui::Button("Save")) { //When clicked it saves the picture as prueba.png
              char sname[1000];
              bits_name.append("_m.png");
              sprintf(sname, bits_name.c_str());
              stbi_write_png(sname, a.width, a.height, 4, b.pixels, 0);
            }
          }

             }
     ImGui::EndGroup();
*/
