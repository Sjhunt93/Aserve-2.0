//
//  ComponentList.h
//  MusicContentAnalysisLibrary
//
//  Created by Samuel Hunt on 27/02/2018.
//
//

#ifndef __MusicContentAnalysisLibrary__ComponentList__
#define __MusicContentAnalysisLibrary__ComponentList__

#include "../JuceLibraryCode/JuceHeader.h"
//This class is basically a scrollable list of components.

class ComponentList : public Component, public Button::Listener {
public:
    
    class Listener {
    public:
        virtual ~Listener () {}
        virtual void addNew (ComponentList * list) {}
    };
    
    
    ComponentList (bool hideAdd = false);
    ~ComponentList ();
    
    void resized ();
    
    void showAddNewButton (bool);
    void setDefaultObjectHeight (float height);
    
    void            addNewComponet (Component * cmp);
    void            removeComponentAtPostion (const int index);
    const int       getTotalComponents ();
    Component *     getComponentAtPostion (const int index);
    
    
    
//add methods for deleting objects..
    
    void buttonClicked (Button*);
    void setListner (Listener * l);
    
    void removeAll ();
private:
    void updateHeight ();
    std::vector<Component *>    components;
    TextButton                  addNew;
    float                       objectHeight;
    Listener *                   listener;
    
    
    //public Viewport
};


class ComponentListViewport : public Component {
    
public:
    
    ComponentListViewport ();
    ~ComponentListViewport ();

    void resized ();
    void paint (Graphics & g);
    ComponentList * getComponentList ();
    
    void update ();

private:
    Viewport viewport;
    ComponentList cl;
    
};


#endif /* defined(__MusicContentAnalysisLibrary__ComponentList__) */
