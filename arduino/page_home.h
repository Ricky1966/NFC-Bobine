//This is a web pages module
#ifndef _PAGE_HOME_H
#define _PAGE_HOME_H
#include "page_header.h"
#include "page_footer.h"
String Page_Home(){
  String page_home = Page_Header();
  page_home += "<body>\n";
  page_home += "<h1 class='display-3' align='center'>ARDUINO NANO ESP32 Web Server</h1>\n";
  page_home += "<nav class='navbar navbar-expand-lg bg-body-tertiary'>\n";
  page_home += "<div class='container-fluid'>\n";
  page_home += "<a class='navbar-brand' href='#'>Navbar</a>\n";
  page_home += "<button class='navbar-toggler' type='button' data-bs-toggle='collapse' data-bs-target='#navbarSupportedContent' aria-controls='navbarSupportedContent' aria-expanded='false' aria-label='Toggle navigation'>\n";
  page_home += "<span class='navbar-toggler-icon'></span>\n";
  page_home += "</button>\n";
  page_home += "<div class='collapse navbar-collapse' id='navbarSupportedContent'>\n";
  page_home += "<ul class='navbar-nav me-auto mb-2 mb-lg-0'>\n";
  page_home += "<li class='nav-item'>\n";
  page_home += "<a class='nav-link active' aria-current='page' href='#'>Home</a>\n";
  page_home += "</li>\n";
  page_home += "<li class='nav-item'>\n";
  page_home += "<a class='nav-link' href='#'>OTA</a>\n";
  page_home += "</li>\n";
  page_home += "<li class='nav-item dropdown'>\n";
  page_home += "<a class='nav-link dropdown-toggle' href='#' role='button' data-bs-toggle='dropdown' aria-expanded='false'>\n";
  page_home += "Dati\n";
  page_home += "</a>\n";
  page_home += "<ul class='dropdown-menu'>\n";
  page_home += "<li><a class='dropdown-item' href='#'>Action</a></li>\n";
  page_home += "<li><a class='dropdown-item' href='#'>Another action</a></li>\n";
  page_home += "<li><hr class='dropdown-divider'></li>\n";
  page_home += "<li><a class='dropdown-item' href='#'>Something else here</a></li>\n";
  page_home += "</ul>\n";
  page_home += "</li>\n";
  page_home += "<li class='nav-item'>\n";
  page_home += "<a class='nav-link disabled' aria-disabled='true'>Disabled</a>\n";
  page_home += "</li>\n";
  page_home += "</ul>\n";
  page_home += "<form class='d-flex' role='search'>\n";
  page_home += "<input class='form-control me-2' type='search' placeholder='Search' aria-label='Search'>\n";
  page_home += "<button class='btn btn-outline-success' type='submit'>Search</button>\n";
  page_home += "</form>\n";
  page_home += "</div>\n";
  page_home += "</div>\n";
  page_home += "</nav>\n";
  page_home += Page_Footer();
  return page_home;
}
#endif
//End of module
