//This is a web pages module
#ifndef _PAGE_OTA_H
#define _PAGE_OTA_H
#include "page_header.h"
#include "page_footer.h"
String Page_OTA(){
  String page_ota = Page_Header();
  page_ota += "<body>\n";
  page_ota += "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>\n";
  page_ota += "<script src='https://cdn.jsdelivr.net/npm/bootstrap@5.3.1/dist/js/bootstrap.bundle.min.js' integrity='sha384-HwwvtgBNo3bZJJLYd8oVXjrBZt8cqVSpeBNS5n7C8IVInixGAoxmnlMuBnhbgrkm' crossorigin='anonymous'></script>\n";
  page_ota += "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>\n";
  page_ota += "<input type='file' name='update'>\n";
  page_ota += "<input class='btn btn-primary mb-3' type='submit' value='Update'>\n";
  page_ota += "</form>\n";
  page_ota += "<div id='prg'>progress: 0%</div>\n";
  page_ota += "<script>\n";
  page_ota += "$('form').submit(function(e){";
  page_ota += "e.preventDefault();";
  page_ota += "var form = $('#upload_form')[0];";
  page_ota += "var data = new FormData(form);";
  page_ota += " $.ajax({";
  page_ota += "url: '/update',";
  page_ota += "type: 'POST',";
  page_ota += "data: data,";
  page_ota += "contentType: false,";
  page_ota += "processData:false,";
  page_ota += "xhr: function() {",
  page_ota += "var xhr = new window.XMLHttpRequest();";
  page_ota += "xhr.upload.addEventListener('progress', function(evt) {";
  page_ota += "if (evt.lengthComputable) {";
  page_ota += "var per = evt.loaded / evt.total;";
  page_ota += "$('#prg').html('progress: ' + Math.round(per*100) + '%');";
  page_ota += "}";
  page_ota += "}, false);";
  page_ota += "return xhr;";
  page_ota += "},";
  page_ota += "success:function(d, s) {";
  page_ota += "console.log('success!')";
  page_ota += "},";
  page_ota += "error: function (a, b, c) {";
  page_ota += "}";
  page_ota += "});";
  page_ota += "});";
  page_ota += "</script>\n";
  page_ota += Page_Footer();
  return page_ota;
}
#endif
//End of module
