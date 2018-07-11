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



Image resize_image(Image a){

Image b;
b.height = (int) (a.height / 4);
b.width =  (int) (a.width/4);
b.pixels = (uint8*)malloc(b.width * b.height * RGBA);

int b_height = 0;
int b_width = 0;
int k = 0; //this variable will control the component copied

for(int x = 0; x< a.height; x=+(4*a.width*4)){ //Jumping 4 rows -> (a.width * 4) --> jumping 4 --> 4 as we have 4 components
  printf("\n X row on pixel : %d ", (4*a.width*4)  );
  for(int y = 0; y<a.width; y=+16) { //Jumping 4 rows each time


    while(k< 4){
      b.pixels[(4* b.width* b_height) + (b_width * 4) + k] = a.pixels[x+y+k];
    }
    k=0;
    b_width++;
  }
  b_height++;
  std::cout << "\nNew image on row : " << b_height;

}

}


ImVec2 resize(int x, int y)
{
    if (x > 200 && y > 200) {
        float d = (x > y ? x : y) / 200;
        return ImVec2( x/d, y/d );
    }
    return ImVec2(x, y);
}

void capture_video( String filename, Mat mask) {
  //Method to capture video from a file
  printf("Testing OpenCV\n");
  printf("Loading Haar-Cascade\n");
  CascadeClassifier faceDetector;
  bool loaded1 = faceDetector.load("data/haarcascade_frontalface_default.xml");
  printf("Training Loaded? %d\n", loaded1 );

  VideoCapture capture(filename); //Get the video
  Mat frame; //This variable will store every frame captured on the Video

  if(!capture.isOpened()){
    throw "Error opening the file";
  }

    namedWindow("w", 1); //open OpenCV window
    for( ; ; ){
      capture >> frame;


      //Call method on frame



      std::vector<Rect> faces; //This will store the values where the faces are at (x,y,w,h), as well as the size of them
      cvtColor(frame, frame, COLOR_BGR2GRAY); //Change color of image from rgb to greyscale
      equalizeHist(frame, frame); //Equialize the histogram of the image so the contrast is deeper and we can detect better the faces

      faceDetector.detectMultiScale(frame, faces, 1.30, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30)); //Detect the faces from src, storing it into a vector with max size

      for( size_t i = 0; i < faces.size(); i++ )
   {
      int xx =0, yy = 0;
  // Just iterate in face region pixel by pixel
     for(int x = faces[i].x; x < faces[i].x+faces[i].width; x++)
                {
              for (int y = faces[i].y; y < faces[i].y+faces[i].height; y++)
                    {

                     //This is core of the trick.
                     // Copy Mask to Original image  If the 0 chan
                     // Proper condition is over all color channels
                     // imageMask.at(xx,yy)[0] < 10
                     // imageMask.at(xx,yy)[1] < 10
                     // imageMask.at(xx,yy)[2] < 10
                                               // Black Color(0,0,0)
                        if (mask.at(xx,yy)[0] < 10)
                        {
                          // Copy to original image on (y,x) places  the pixel of xx,yy mask
                          frame.at(y,x)[0] =  mask.at(xx,yy)[0];
                          frame.at(y,x)[1] =  mask.at(xx,yy)[1];
                          frame.at(y,x)[2] =  mask.at(xx,yy)[2];
                          }
                          // Iterate in mask x
                  xx =xx+1;
            }
      // iterate next row of imageMask
      xx = 0;
      yy =yy+1;
      }
   }

///////////////////////////////////////////////////////////////////////////////////////////////////////////


      if(frame.empty()){
        break; //Stop capturing video once it is done
      }
      imshow("w", frame); //Use openCV window to show the frame
      waitKey(20); //Wait til next frame
    }
    waitKey(0); //Key press to close window releasing and destroying the window
}



void mask_face(Image a, Mat mask){
  if(mask.empty()) printf("Error abriendo el archivo\n");
  printf("Testing OpenCV\n");
  printf("Loading Haar-Cascade\n");
  CascadeClassifier faceDetector;
  bool loaded1 = faceDetector.load("data/haarcascade_frontalface_default.xml");
  printf("Training Loaded? %d\n", loaded1 );
//  b = resize_image(b, 100, 100);


  std::vector<Rect> faces; //This will store the values where the faces are at (x,y,w,h), as well as the size of them
  cvtColor(original, original, COLOR_BGR2GRAY); //Change color of image from rgb to greyscale
  equalizeHist(original, original); //Equialize the histogram of the image so the contrast is deeper and we can detect better the faces

  faceDetector.detectMultiScale(original, faces, 1.30, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30)); //Detect the faces from src, storing it into a vector with max size

  std::cout << "\nFaces detected " << faces.size();
  for(int i = 0; i< faces.size(); i++){
    std::cout << "\nFace " << i << faces[i];
    std::cout << "\nFace_X " << i << faces[i].x;
      std::cout << "\nFace_y " << i << faces[i].y;
  }

  /*  for(int x = faces[1].y; x < faces[1].y + final_size; x++){
      for(int y = faces[1].x*4 ; y < (faces[1].x + final_size)*4 ; y++){
*/

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

    Image b;

    std::string tmp;
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
            if (ImGui::Button("Transform"))
                ImGui::OpenPopup("select 0");
             tmp = image_select(0); //opens the file list and assigns to tmp
            if (tmp != "") {
                fnames[0] = tmp;

                reset_image(&b);
                //Method to capture video from a file
            /*    a.pixels = stbi_load(fnames[0].c_str(), &a.width, &a.height, &a.n, RGBA);
                original = imread(fnames[0].c_str()); //Creating Mat object over which the mask will be applied
                texture_image(&a);*/
              }

            if(ImGui::Button("Reset")){

                reset_image(&b);


                free_image(&b);
                free(b.pixels);

            }
        }

           ImGui::EndGroup();
           ImGui::SameLine();

               ImGui::BeginGroup();
              {

                   if(b.texture_loaded){

                   ImGui::Image((void*)b.texture, resize(b.width, b.height));
                   if(ImGui::Button("Transform")) capture_video(tmp , mask );

                   }

                   if(ImGui::Button("Select mask"))
                       ImGui::OpenPopup("select 1");


                   std::string tmp_2 = image_select(1);
                   if(tmp_2 !=""){
                   fnames[0] = tmp_2;
                   reset_image(&b);


                   b.pixels = stbi_load(fnames[0].c_str(), &b.width, &b.height, &b.n, RGBA);
                    mask = imread(fnames[0].c_str()); //Creating Mat object over which the mask will be applied
                   texture_image(&b);
                 /*  if((b.width * b.height) > 19200){
                   b = resize_4(b);
                   texture_image(&b);
                 }*/
                   }

              }
               ImGui::EndGroup();

           ImGui::SameLine();

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

    free_image(&b);

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
