#include "TextRec.h"

#include <cmath>

template<class T>
void nafy::TextRecBase<T>::init() {
    margin = 0;
    regenMove();
}

template<class T>
nafy::TextRecBase<T>::TextRecBase() {
    init();
}

template<class T>
nafy::TextRecBase<T>::TextRecBase(const T &text, const Rectangle &box):
    text(text), box(box) {
    init();
}

template<class T>
nafy::TextRecBase<T>::TextRecBase(T &&text, Rectangle &&box):
    text(std::move(text)), box(std::move(box)) {
    init();
}

template<class T>
void nafy::TextRecBase<T>::reposText() {
    // We use the rectangle as a point of origin
    text.setX(box.getX() + move.xy);
    text.setY(box.getY() + move.xy);
    text.setWrappingWidth(box.getWidth() + move.width);
    text.setOverflowHeight(box.getHeight() + move.height);
}

template<class T>
int nafy::TextRecBase<T>::calcMove(unsigned int radius) {
    // Equivalent to 1 - sin(45)
    constexpr float distBase = 1 - std::sqrt(2) / 2.0f;
    // Will break with very big numbers, like 2^31-1 big
    return std::round(margin + radius * distBase);
}

template<class T>
void nafy::TextRecBase<T>::regenMove() {
    move.xy = calcMove(box.getCornerRadiusTopLeft());
    // The bottom right point radius will have an effect on both
    // the width and the height, however the top right point radius will
    // only effect the width and the bottom left radius will only effect
    // the height. Hence, we take the min of the two for each one.
    // Also deduct the x/y move, because it's supposed to be decreasing
    // the size, not moving it.
    const int dual = calcMove(box.getCornerRadiusBottomRight());
    move.width = -std::min(calcMove(box.getCornerRadiusTopRight()), dual) - move.xy;
    move.height = -std::min(calcMove(box.getCornerRadiusBottomLeft()), dual) - move.xy;
}

template<class T>
void nafy::TextRecBase<T>::setMargin(unsigned int margin) {
    this->margin = margin;
    afRadius();
}

template<class T>
unsigned int nafy::TextRecBase<T>::getMargin() {
    return margin;
}

template<class T>
typename nafy::TextRecBase<T>::move_t nafy::TextRecBase<T>::getMove() {
    return move;
}

template<class T>
void nafy::TextRecBase<T>::setX(int x) {
    box.setX(x);
    text.setX(x + move.xy);
}
template<class T>
void nafy::TextRecBase<T>::setY(int y) {
    box.setY(y);
    text.setY(y + move.xy);
}
template<class T>
void nafy::TextRecBase<T>::setWidth(unsigned int width) {
    text.setWrappingWidth(width + move.width);
    box.setWidth(width);
    text.setX(box.getX() + move.xy);
}
template<class T>
void nafy::TextRecBase<T>::setHeight(unsigned int height) {
    text.setOverflowHeight(height + move.height);
    box.setHeight(height);
    text.setY(box.getY() + move.xy);
}

template<class T>
void nafy::TextRecBase<T>::afRadius() {
    regenMove();
    reposText();
}
template<class T>
void nafy::TextRecBase<T>::setCornerRadius(unsigned int radius) {
    box.setCornerRadius(radius);
    afRadius();
}
template<class T>
void nafy::TextRecBase<T>::setCornerRadius(int index, unsigned int radius) {
    box.setCornerRadius(index, radius);
    afRadius();
}
template<class T>
void nafy::TextRecBase<T>::setCornerRadiusTopLeft(unsigned int radius) {
    box.setCornerRadiusTopLeft(radius);
    afRadius();
}
template<class T>
void nafy::TextRecBase<T>::setCornerRadiusTopRight(unsigned int radius) {
    box.setCornerRadiusTopRight(radius);
    afRadius();
}
template<class T>
void nafy::TextRecBase<T>::setCornerRadiusBottomRight(unsigned int radius) {
    box.setCornerRadiusBottomRight(radius);
    afRadius();
}
template<class T>
void nafy::TextRecBase<T>::setCornerRadiusBottomLeft(unsigned int radius) {
    box.setCornerRadiusBottomLeft(radius);
    afRadius();
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
void nafy::TextRecBase<T>::getCornerRadii(unsigned int data[4]) {
    box.getCornerRadii(data);
}
template<class T>
unsigned int nafy::TextRecBase<T>::getCornerRadius(int index) {
    return box.getCornerRadius(index);
}
template<class T>
unsigned int nafy::TextRecBase<T>::getCornerRadiusTopLeft() {
    return box.getCornerRadiusTopLeft();
}
template<class T>
unsigned int nafy::TextRecBase<T>::getCornerRadiusTopRight() {
    return box.getCornerRadiusTopRight();
}
template<class T>
unsigned int nafy::TextRecBase<T>::getCornerRadiusBottomRight() {
    return box.getCornerRadiusBottomRight();
}
template<class T>
unsigned int nafy::TextRecBase<T>::getCornerRadiusBottomLeft() {
    return box.getCornerRadiusBottomLeft();
}

template<class T>
nafy::Color &nafy::TextRecBase<T>::getColor() {
    return box.getColor();
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
