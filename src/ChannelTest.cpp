#include <iostream>
#include <HttpServerTransport.h>
#include <ctemplate/template.h>

#include "LatencyStatManager.h"

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

    unichannel::LatencyStatManager::Ptr manager(new unichannel::LatencyStatManager(2));
    manager->start();
    
    unichannel::HttpServerTransport server("8091", manager, "/var/www/html/quickNote");
    server.start();

    manager->onLatencyUpdate("1", 897669200000, 4.5);
    boost::this_thread::sleep(boost::posix_time::seconds(2));
    manager->onLatencyUpdate("1", 898669200000, 2);
    boost::this_thread::sleep(boost::posix_time::seconds(2));
    manager->onLatencyUpdate("1", 899669200000, 8);
    manager->onLatencyUpdate("1", 899769200000, 1.2);
    manager->onLatencyUpdate("1", 899869200000, 5);
    boost::this_thread::sleep(boost::posix_time::seconds(2));
    manager->onLatencyUpdate("1", 899969200000, 19);
    boost::this_thread::sleep(boost::posix_time::seconds(2));
    manager->onLatencyUpdate("1", 899979200000, 2.8);


    while (true) {
        // Keep Running it
        boost::this_thread::sleep(boost::posix_time::seconds(2));
    }

    return 0;
}
