#pragma once
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;


typedef struct Vector2 {
	float x;
	float y;
}Vector2;

typedef struct Matrix3x3 {

	float m[3][3];

} Matrix3x3;

typedef struct Point {
	float left;
	float top;
	float right;
	float bottom;
}Point;

typedef struct Line {
	Vector2 begin;
	Vector2 end;
}Line;

typedef struct Vertex {
	Vector2 leftTop;
	Vector2 rightTop;
	Vector2 leftBottom;
	Vector2 rightBottom;
}Vertex;

typedef struct Affine {
	Vector2 scale;
	float theta;
	Vector2 translate;
}Affine;

typedef struct LeftAndSize {
	float left;
	float top;
	float width;
	float height;
}LeftAndSize;

typedef struct LinePosition {
	Line lateral;
	Line vert;
}LinePosition;

typedef struct Speed {
	float scale;
	float rotate;
	float translate;
}Speed;
