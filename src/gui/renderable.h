#ifndef RENDERABLE_H_INCLUDED
#define RENDERABLE_H_INCLUDED

/*
* A single renderable node
* TODO: Refactor this to node, etc.
*/

namespace nafy {
    class renderable {
    public:
        virtual ~renderable() = 0;
        virtual void render() = 0;
    };
}

#endif
