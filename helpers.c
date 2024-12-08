#include "helpers.h"
#include<math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
  //copy image
  RGBTRIPLE copy[height][width];

  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      copy[i][j] = image[i][j];
    }
  }

  //pixels aanpassen
  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      int average = (copy[i][j].rgbtRed + copy[i][j].rgbtGreen + copy[i][j].rgbtBlue)/3;
      image[i][j].rgbtRed = average;
      image[i][j].rgbtGreen = average;
      image[i][j].rgbtBlue = average;
    }
  }
  return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
  //pixels aanpassen door horizontaal te swappen
  for (int j = 0; j < width; j++){
    for (int i= 0; i < height/2 ; i++){
      //Red
      int temp = image[i][j].rgbtRed;
      image[i][j].rgbtRed = image[height -i][j].rgbtRed; 
      image[height - i][j].rgbtRed = temp;
      //Green
      temp = image[i][j].rgbtGreen;
      image[i][j].rgbtGreen = image[height -i][j].rgbtGreen;
      image[height - i][j].rgbtGreen = temp;
      //Blue
      temp = image[i][j].rgbtBlue;
      image[i][j].rgbtBlue = image[height -i][j].rgbtBlue;
      image[height - i][j].rgbtBlue = temp;
    }
  }
  return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
 //copy image
 RGBTRIPLE copy[height][width];
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      copy[i][j] = image[i][j];
    }
  }

  //average vn de pixels rondom verkrijgen
  for(int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      
      // variabelen declareren
      float sumRed;
      float sumGreen;
      float sumBlue;
      int counter;
        sumRed = sumGreen = sumBlue = counter = 0;

      //check of er pixels rondom zijn
         //zo ja, tel ze op bij de som
         //zo nee, sla ze over
      for(int k = - 1; k < 2; k++){ // waarde van k is -1 van i tot 2 van i
        for (int l = -1; l < 2; l++){ // idem voor l
          if (i + k < 0 || i +k  >= height){
            continue;
          }
          if (j + l < 0 || j + l >= width){
            continue;
          }
          sumRed += copy[i+k][j+l].rgbtRed;
          sumGreen += copy[i+k][j+l].rgbtGreen;
          sumBlue += copy[i+k][j+l].rgbtBlue;
          counter++;
        }
      }
      //average kopieren naar originele foto
      image[i][j].rgbtRed = round(sumRed  / counter);
      image[i][j].rgbtGreen = round(sumGreen/counter);
      image[i][j].rgbtBlue = round(sumBlue / counter);
    }
  }
  return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
  //copy image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++){
      for (int j = 0; j < width; j++){
      copy[i][j] = image[i][j];
      }
    }
  
  // kernels invullen
  int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
  int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

  //variabelen declareren
  float GX_Red, GX_Green, GX_Blue;
  float GY_Red, GY_Green, GY_Blue;
  float Sobel_Operator_Red, Sobel_Operator_Blue, Sobel_Operator_Green;

  //pixel selection
  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      
      //zet de variabelen naar 0 om garbage values te vermijden
      GX_Red = GY_Red = GX_Green = GY_Green = GX_Blue = GY_Blue = 0.0;
      
      //kijken per pixel welke pixels er rond zitten
      for (int k = 0; k < 3; k++){
        for(int l = 0; l < 3; l++){
          
          //detect if outside image
          int d = i + k - 1 ;
          int e = j + l - 1;

          if (d < 0 || d >= height){
           continue;
          }
          if (e < 0 || e >= width){
            continue;
          }
         
         GX_Red += copy[d][e].rgbtRed * gx[k][l];
         GX_Green += copy[d][e].rgbtGreen* gx[k][l];
         GX_Blue += copy[d][e].rgbtBlue * gx[k][l];
        
         GY_Red += copy[d][e].rgbtRed * gx[k][l];
         GY_Green += copy[d][e].rgbtGreen* gx[k][l];
         GY_Blue += copy[d][e].rgbtBlue * gx[k][l];
        }
      }
      
      //Sobel Operator berekenen
      Sobel_Operator_Red = fmin(sqrt(GX_Red * GX_Red + GY_Red * GY_Red), 250.0f);
      Sobel_Operator_Blue = fmin(sqrt(GX_Blue * GX_Blue + GY_Blue * GY_Blue), 250.0f);
      Sobel_Operator_Green = fmin(sqrt(GX_Green * GX_Green + GY_Green * GY_Green), 250.0f);
        
      //sobel operator in de pixels van de oorspronkelijke afbeelding stoppen
      image[i][j].rgbtRed = (uint8_t)round(Sobel_Operator_Red);
      image[i][j].rgbtGreen = (uint8_t)round(Sobel_Operator_Green);
      image[i][j].rgbtBlue = (uint8_t)round(Sobel_Operator_Blue);
    }
  }
  return;
}
