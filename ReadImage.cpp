#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>

#include "Utilities/Utils.h"

using namespace std;
using namespace cv;

int main(int argc, char** varg)
{
  Mat image;
  ofstream outStream;
  if(argc < 3)
  {
    cerr << "usage: cmd image outfile" << endl;
    return 1;
  }

  char* outText;
  tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
  if(api->Init("/aux/brian/local/share/", "eng",
	       tesseract::OEM_TESSERACT_CUBE_COMBINED))
  {
    cerr << "Could not initialize tesseract" << endl;
    return 1;
  }

  Pix* image = pixRead(varg[1]);
  api->SetImage(image);

  outText = api->GetUTF8Text();

  Utils::FileOpen(outStream, varg[2]);
  outStream << Utils::Format(outText) << endl;
  Utils::FileClose(outStream);

  api->End();
  delete [] outText;
  pixDestroy(&image);

  return 0;
}
