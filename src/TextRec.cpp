#include "TextRec.h"

#include <cmath>

template<class T>
nafy::TextRecBase<T>::TextRecBase(): margin(0) {
    regenMove();
}

template<class T>
nafy::TextRecBase<T>::TextRecBase(const T &text, const Rectangle &box):
    text(text), box(box), margin(0) {
    regenMove();
}

template<class T>
void nafy::TextRecBase<T>::reposText() {
    const int move = getMove();
    text.setX(box.getX() + move);
    text.setY(box.getY() + move);
    text.setWrappingWidth(box.getWidth() - move * 2);
    text.setOverflowHeight(box.getHeight() - move * 2);
}

template<class T>
void nafy::TextRecBase<T>::regenMove() {
    constexpr float distBase = 1 - std::sqrt(2) / 2.0f;
    // Will break with very big numbers, like 2^31 big
    move = margin + box.getCornerRadius() * distBase;
}

template<class T>
void nafy::TextRecBase<T>::setMargin(unsigned int margin) {
    this->margin = margin;
    regenMove();
    reposText();
}

template<class T>
unsigned int nafy::TextRecBase<T>::getMargin() {
    return margin;
}

template<class T>
int nafy::TextRecBase<T>::getMove() {
    return move;
}

template<class T>
void nafy::TextRecBase<T>::setX(int x) {
    box.setX(x);
    text.setX(x + getMove());
}
template<class T>
void nafy::TextRecBase<T>::setY(int y) {
    box.setY(y);
    text.setY(y + getMove());
}
template<class T>
void nafy::TextRecBase<T>::setWidth(unsigned int width) {
    text.setWrappingWidth(width - getMove() * 2);
    box.setWidth(width);
    text.setX(box.getX() + getMove());
}
template<class T>
void nafy::TextRecBase<T>::setHeight(unsigned int height) {
    text.setOverflowHeight(height - getMove() * 2);
    box.setHeight(height);
    text.setY(box.getY() + getMove());
}
template<class T>
void nafy::TextRecBase<T>::setCornerRadius(unsigned int radius) {
    box.setCornerRadius(radius);
    regenMove();
    reposText();
}

template<class T>
int nafy::TextRecBase<T>::getX() {
    return box.getX();
}
template<class T>
int nafy::TextRecBase<T>::getY() {
    return box.getY();
}
template<class T>
unsigned int nafy::TextRecBase<T>::getWidth() {
    return box.getWidth();
}
template<class T>
unsigned int nafy::TextRecBase<T>::getHeight() {
    return box.getHeight();
}
template<class T>
unsigned int nafy::TextRecBase<T>::getCornerRadius() {
    return box.getCornerRadius();
}

template<class T>
void nafy::TextRecBase<T>::generate() {
    box.generate();
    text.generate();
}

template<class T>
void nafy::TextRecBase<T>::render() {
    box.render();
    text.render();
}

template<class T>
nafy::Rectangle &nafy::TextRecBase<T>::getBox() {
    return box;
}
template<class T>
T &nafy::TextRecBase<T>::getText() {
    return text;
}



template class nafy::TextRecBase<Text>;
template class nafy::TextRecBase<TextCrawl>;
