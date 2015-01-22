#include <iostream>
#include <HttpServerTransport.h>
#include <ctemplate/template.h>

int main() {
    std::cout << " testing " << std::endl;

//    //ctemplate feature
//    ctemplate::TemplateDictionary rootDict("example");
//    rootDict.SetValue("TITLE", "HTML Page from CTemplates");
//    
//    // Set CSS Files
//    ctemplate::TemplateDictionary* sectionCSSDict1 = rootDict.AddSectionDictionary("CSS");
//    sectionCSSDict1->SetValue("CSSFILE", "bootstrap/css/bootstrap.min.css");
//    
//    ctemplate::TemplateDictionary* sectionCSSDict2 = rootDict.AddSectionDictionary("CSS");
//    sectionCSSDict2->SetValue("CSSFILE", "bootstrap/css/bootstrap-responsive.min.css");
//    
//    rootDict.SetValue("HEADING", "Hurraayyyy!!!!!");
//    rootDict.SetValue("FOOTER", "We are awesome!!!!!");
//    
//    
//    // For now, assume everyone lives in CA.
//    // (Try running the program with a 0 here instead!)
////    if (1) {
////        dict.ShowSection("IN_CA");
////    }
//    std::string output;
//    ctemplate::ExpandTemplate("../templates/myhtml.tpl", ctemplate::DO_NOT_STRIP, &rootDict, &output);
//    std::cout << output << std::endl ;

        unichannel::HttpServerTransport server("8091", "/var/www/html/quickNote");
        server.start();
    return 0;
}
