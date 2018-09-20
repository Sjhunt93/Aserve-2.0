//
//  LiveTextParser.h
//  Aserve 2
//
//  Created by Samuel Hunt on 17/07/2018.
//
//

#ifndef __Aserve_2__LiveTextParser__
#define __Aserve_2__LiveTextParser__

#include "../../JuceLibraryCode/JuceHeader.h"

class LiveTextParser {
public:
    
    enum eCommands {
        eNull,
        eOsc,
        eLpf,
        eTotal
    };
    
    struct Parsed {
        double data[4]; //there is a max of 4
        String error;
        int type;
    };
    
    static Parsed parse (String txt)
    {
        txt = txt.removeCharacters(" ");
        
        if (txt.startsWith("aserveOscillator(")) {
            return parseAserveOsc(txt.fromFirstOccurrenceOf("aserveOscillator(", false, false));
        }
        if (txt.startsWith("aserveLPF(")) {
            return parseLPF(txt.fromFirstOccurrenceOf("aserveLPF(", false, false));
        }
        Parsed p;
        p.error = "Cannot Parse - " + txt;
        p.type = eNull;
        return p;
    }
    
    static Parsed parseAserveOsc (String txt)
    {
        Parsed parsed;
        String param1 = txt.upToFirstOccurrenceOf(",", false, false);
        txt = txt.fromFirstOccurrenceOf(",", false, false);
        
        String param2 = txt.upToFirstOccurrenceOf(",", false, false);
        txt = txt.fromFirstOccurrenceOf(",", false, false);
        
        String param3 = txt.upToFirstOccurrenceOf(",", false, false);
        txt = txt.fromFirstOccurrenceOf(",", false, false);
        
        String param4 = txt.upToFirstOccurrenceOf(")", false, false);
        
        if (!txt.endsWith(";")) {
            parsed.error = "Missing Semi-Colon";
        }
        
        if (param1.isEmpty()) {
            parsed.error = "Parameter 1 is empty";
        }
        if (param2.isEmpty()) {
            parsed.error = "Parameter 2 is empty";
        }
        if (param3.isEmpty()) {
            parsed.error = "Parameter 3 is empty";
        }
        if (param4.isEmpty()) {
            parsed.error = "Parameter 4 is empty";
        }

        parsed.data[0] = param1.getIntValue();
        parsed.data[1] = param2.getFloatValue();
        parsed.data[2] = param3.getFloatValue();
        parsed.data[3] = param4.getIntValue();
        parsed.type = eOsc;
        return parsed;
    }
    static Parsed parseLPF (String txt)
    {
        Parsed parsed;
        
        if (!txt.endsWith(";")) {
            parsed.error = "Missing Semi-Colon";
        }
        
        String value = txt.upToFirstOccurrenceOf(")", false, false);
        if (value.isEmpty()) {
            parsed.error = "Parameter 1 is empty";
        }
        parsed.data[0] = value.getFloatValue();
        parsed.type = eLpf;
        return parsed;
    }
    
};

#endif /* defined(__Aserve_2__LiveTextParser__) */
