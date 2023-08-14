//This is a web pages module
#ifndef _PAGEHEADER_H
#define _PAGEHEADER_H
String Page_Header(){
String page_header = "<!DOCTYPE html>\n";
  page_header +="<html lang='en'>\n";
  page_header +="<head>\n";
  page_header +="<meta charset='utf-8'>\n";
  page_header +="<meta name='viewport' content='width=device-width, initial-scale=1'>\n";
  page_header +="<title>ARDUINO NANO ESP32</title>\n";
  page_header +="<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.1/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-4bw+/aepP/YC94hEpVNVgiZdgIC5+VKNBQNGCHeKRQN+PtmoHDEXuppvnDJzQIu9' crossorigin='anonymous'>\n";
  page_header +="</head>\n";
  return page_header;
}
#endif
//End of module
