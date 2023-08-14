//This is a web pages module
#ifndef _PAGE_LOGIN_H
#define _PAGE_LOGIN_H
#include "page_header.h"
#include "page_footer.h"
String Page_Login(){
  String page_login = Page_Header();
  page_login += "<body>\n";
  page_login += "<div class='container' align='center'>\n";
  page_login += "<center><font size=4><b>ARDUINO NANO ESP32 Login Page</b></font></center>\n";
  page_login += "<form name='loginForm'>\n";
  page_login += "<div class='row g-3 align-items-center'>\n";
  page_login += "<div class='col-auto' align='center'>\n";
  page_login += "<label for='userid' class='col-form-label'>Username :</label>\n";
  page_login += "</div>\n";
  page_login += "<div class='col-auto' align='center'>\n";
  page_login += "<input type='text' id='userid' name='userid' class='form-control' aria-describedby='userHelpInline'>\n";
  page_login += "</div>\n";
  page_login += "<div class='col-auto'>\n";
  page_login += "<span id='userHelpInline' class='form-text' align='center'>\n";
  page_login += "Must be 5-20 characters long.";
  page_login += "</span>\n";
  page_login += "</div>\n";
  page_login += "</div>\n";
  page_login += "<div class='row g-3 align-items-center'>\n";
  page_login += "<div class='col-auto' align='center'>\n";
  page_login += "<label for='pwd' class='col-form-label'>Password :</label>\n";
  page_login += "</div>\n";
  page_login += "<div class='col-auto' align='center'>\n";
  page_login += "<input type='password' id='pwd' name='pwd' class='form-control' aria-describedby='passwordHelpInline'>\n";
  page_login += "</div>\n";
  page_login += "<div class='col-auto' align='center'>\n";
  page_login += "<span id='passwordHelpInline' class='form-text'>\n";
  page_login += "Must be 5-20 characters long.";
  page_login += "</span>\n";
  page_login += "</div>\n";
  page_login += "</div>\n";
  page_login += "<input type='submit' align='center' class='btn btn-primary mb-3' onclick='check(this.form)'>\n";
  page_login += "</form>\n";
  page_login += "</div>\n";
  page_login += "<script>\n";
  page_login += "function check(form)";
  page_login += "{";
  page_login += "if(form.userid.value=='admin' && form.pwd.value=='admin')";
  page_login += "{";
  page_login += "window.open('/serverIndex')";
  page_login += "}";
  page_login += "else";
  page_login += "{";
  page_login += " alert('Error Password or Username')/*displays error message*/";
  page_login += "}";
  page_login += "}";
  page_login += "</script>\n";
  page_login += Page_Footer();
  return page_login;
}
#endif
//End of module
