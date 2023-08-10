//This is a web pages module

#ifndef _LOGINPAGE_H
#define _LOGINPAGE_H

/*
 * Login page
 */

const char* loginIndex =
"<!doctype html>"
"<html lang='en'>"
  "<head>"
    "<meta charset='utf-8'>"
    "<meta name='viewport' content='width=device-width, initial-scale=1'>"
    "<title>ARDUINO NANO ESP32</title>"
    "<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.1/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-4bw+/aepP/YC94hEpVNVgiZdgIC5+VKNBQNGCHeKRQN+PtmoHDEXuppvnDJzQIu9' crossorigin='anonymous'>"
  "</head>"
  "<body>"
    "<div class='container' align='center'>"
        "<center><font size=4><b>ARDUINO NANO ESP32 Login Page</b></font></center>"
    "<form name='loginForm'>"
    "<div class='row g-3 align-items-center'>"
      "<div class='col-auto' align='center'>"
        "<label for='userid' class='col-form-label'>Username :</label>"
      "</div>"
      "<div class='col-auto' align='center'>"
        "<input type='text' id='userid' name='userid' class='form-control' aria-describedby='userHelpInline'>"
      "</div>"
      "<div class='col-auto'>"
        "<span id='userHelpInline' class='form-text' align='center'>"
          "Must be 5-20 characters long."
        "</span>"
      "</div>"
      "</div>"
      "<div class='row g-3 align-items-center'>"
      "<div class='col-auto' align='center'>"
        "<label for='pwd' class='col-form-label'>Password :</label>"
      "</div>"
      "<div class='col-auto' align='center'>"
        "<input type='password' id='pwd' name='pwd' class='form-control' aria-describedby='passwordHelpInline'>"
      "</div>"
      "<div class='col-auto' align='center'>"
        "<span id='passwordHelpInline' class='form-text'>"
          "Must be 5-20 characters long."
        "</span>"
      "</div>"
      "</div>"
      "<input type='submit' align='center' class='btn btn-primary mb-3' onclick='check(this.form)'>"
      "</form>"
    "</div>"
  "<script>"
    "function check(form)"
    "{"
    "if(form.userid.value=='admin' && form.pwd.value=='admin')"
    "{"
    "window.open('/serverIndex')"
    "}"
    "else"
    "{"
    " alert('Error Password or Username')/*displays error message*/"
    "}"
    "}"
  "</script>"
  "</body>"
"</html>";
#endif
//End of module
