//
//  ComponentList.cpp
//  MusicContentAnalysisLibrary
//
//  Created by Samuel Hunt on 27/02/2018.
//
//

#include "ComponentList.h"



ComponentList::ComponentList (bool hideAdd)
{
    addAndMakeVisible(addNew);
    addNew.addListener(this);
    addNew.setButtonText("+");
    addNew.setVisible(!hideAdd);
    objectHeight = 30.0;
 
    listener = nullptr;
}
ComponentList::~ComponentList ()
{
    for (int i = 0; i < components.size(); i++) {
        delete components[i];
    }
}

void ComponentList::resized ()
{
    const float width = getWidth() - 8.f;
    float runningY = 4.f;
    
    for (int i = 0; i < components.size(); i++) {
        components[i]->setBounds(4.f, runningY, width, objectHeight - 2.f);
        runningY += objectHeight;
    }
    addNew.setBounds(getWidth() * 0.5 - (objectHeight * 0.5), runningY, objectHeight, objectHeight);
    
    const int h = getHeight();
}



void ComponentList::showAddNewButton (bool state)
{
    addNew.setVisible(state);
    
    updateHeight();

}
void ComponentList::addNewComponet (Component * cmp)
{
    components.push_back(cmp);
    addAndMakeVisible(cmp);

    updateHeight();

}
const int ComponentList::getTotalComponents ()
{
    return components.size();
}
Component * ComponentList::getComponentAtPostion (const int index)
{
    if (index >= 0 && index <= components.size()) {
        return components[index];
    }
    else {
        jassertfalse;
    }
    return nullptr;
}
void ComponentList::removeComponentAtPostion (const int index)
{
    std::vector<Component *> newComp;
    for (int i = 0; i < components.size(); i++) {
        if (i == index) {
            removeChildComponent(components[i]);
            delete components[i];
        }
        else {
            newComp.push_back(components[i]);
        }
    }
    components = newComp;
}
void ComponentList::removeAll ()
{
    for (int i = 0; i < components.size(); i++) {
        removeChildComponent(components[i]);
        delete components[i];
    }
    components.clear();
}

void ComponentList::setDefaultObjectHeight (float height)
{
    objectHeight = height;
    updateHeight();
}

void ComponentList::buttonClicked (Button* btn)
{
    if (btn == &addNew && listener != nullptr) {
        listener->addNew(this);
    }
}

void ComponentList::setListner (Listener * l)
{
    listener = l;
}

void ComponentList::updateHeight ()
{
    float newHeight = (components.size() + !!addNew.isVisible() ) * objectHeight;
    setSize(getWidth(), newHeight + 8.0);
  //  resized();
    
}

ComponentListViewport::ComponentListViewport () : cl(true)
{
    addAndMakeVisible(viewport);
    viewport.setViewedComponent(&cl, false);
}
ComponentListViewport::~ComponentListViewport ()
{
    
}

void ComponentListViewport::paint (Graphics & g)
{
    g.fillAll(Colours::lightgrey.darker());
}
void ComponentListViewport::resized ()
{
 
    viewport.setBounds(0, 0, getWidth(), getHeight());
    cl.setBounds(0, 0, getWidth(), cl.getHeight());

}

ComponentList * ComponentListViewport::getComponentList ()
{
    return &cl;
}
