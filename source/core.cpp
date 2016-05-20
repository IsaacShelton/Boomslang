
#include <string>
#include <vector>
#include "../include/core.h"

using namespace std;

void load_core(Environment& environment){
    // Base String Template
    environment.global.templates.push_back( Template{"String"} );
    environment.global.methods.push_back( Method{"String.output",&environment.global, std::vector<MethodArgument>(),"void"} );
    environment.global.methods.push_back( Method{"String.print",&environment.global, std::vector<MethodArgument>(),"void"} );

    // Base Number Template
    environment.global.templates.push_back( Template{"Number"} );
    environment.global.methods.push_back( Method{"Number.output",&environment.global, std::vector<MethodArgument>(),"void"} );
    environment.global.methods.push_back( Method{"Number.print",&environment.global, std::vector<MethodArgument>(),"void"} );
}
