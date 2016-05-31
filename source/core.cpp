
#include <string>
#include <vector>
#include "../include/core.h"

using namespace std;

void load_core(Environment& environment){
    // Base String Template
    environment.global.templates.push_back( Template{"String", true} );
    environment.global.children.push_back( new Scope{TEMPLATE_PREFIX + "String", &environment.global} );
    environment.global.methods.push_back( Method{"String.output",&environment.global, std::vector<MethodArgument>(),"void"} );
    environment.global.methods.push_back( Method{"String.print",&environment.global, std::vector<MethodArgument>(),"void"} );
    environment.global.methods.push_back( Method{"String.input",&environment.global, std::vector<MethodArgument>(),"void"} );
    environment.global.methods.push_back( Method{"String.toNumber",&environment.global, std::vector<MethodArgument>(),"Number"} );

    // Base Number Template
    environment.global.templates.push_back( Template{"Number", true} );
    environment.global.children.push_back( new Scope{TEMPLATE_PREFIX + "Number", &environment.global} );
    environment.global.methods.push_back( Method{"Number.output",&environment.global, std::vector<MethodArgument>(),"void"} );
    environment.global.methods.push_back( Method{"Number.print",&environment.global, std::vector<MethodArgument>(),"void"} );
    environment.global.methods.push_back( Method{"Number.toString",&environment.global, std::vector<MethodArgument>(),"String"} );
}
