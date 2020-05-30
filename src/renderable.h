#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

namespace nafy {
    class renderable {
    public:
        renderable() = default;
        virtual ~renderable() = 0;
        virtual void render() = 0;
    };
}

#endif
