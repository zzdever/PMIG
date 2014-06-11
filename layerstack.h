#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include <QStack>

struct Layer{
    bool isVisible;
};

//class QStack<int>;

class LayerStack:public QStack<int>
{
public:
    LayerStack();

    //setVisible(int layerNum);
};

#endif // LAYERSTACK_H
