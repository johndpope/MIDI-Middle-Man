#include <iostream>
#include "CoreMIDI/MIDIServices.h"
using namespace std;

int main(int argc, const char * argv[])
{

    std::cout << MIDIGetNumberOfDevices() << endl << endl;
    
    std::cout << MIDIGetDevice(1);
    
    MIDIInputPortCreate(<#MIDIClientRef client#>, <#CFStringRef portName#>, <#MIDIReadProc readProc#>, <#void *refCon#>, <#MIDIPortRef *outPort#>)
    
    OSStatus MIDIInputPortCreate (
                                  MIDIClientRef  client,
                                  CFStringRef    portName,
                                  MIDIReadProc   readProc,
                                  void           *refCon,
                                  MIDIPortRef    *outPort
                                  );
    
    
    return 0;
}



