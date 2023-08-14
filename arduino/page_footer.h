//This is a web pages module
#ifndef _PAGEFOOTER_H
#define _PAGEFOOTER_H
String Page_Footer(){
String page_footer = "<footer class='bg-light text-center text-lg-start'>\n";
  page_footer += "<!-- Copyright -->\n";
  page_footer += "<div class='text-center p-3' style='background-color: rgba(0, 0, 0, 0.2);'>\n";
  page_footer += "<a class='text-dark' href='https://mdbootstrap.com/'>\n";
  page_footer += "</a>\n";
  page_footer += "<!-- Copyright -->\n";
  page_footer += "</footer>\n";
  page_footer +="<script src='https://cdn.jsdelivr.net/npm/bootstrap@5.3.1/dist/js/bootstrap.bundle.min.js' integrity='sha384-HwwvtgBNo3bZJJLYd8oVXjrBZt8cqVSpeBNS5n7C8IVInixGAoxmnlMuBnhbgrkm' crossorigin='anonymous'></script>\n";
  page_footer +="</body>\n";
  page_footer +="</html>\n";
  return page_footer;
}
#endif
//End of module
