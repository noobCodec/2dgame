#include "shape.h"


ShapeCircle shape_circle(float x, float y, float r)
{
    ShapeCircle c;
    c.x = x;
    c.y = y;
    c.r = r;
    return c;
}

ShapeRect shape_rect_from_sdl_rect(SDL_Rect r)
{
    ShapeRect sr;
    gfc_rect_set(sr,r.x,r.y,r.w,r.h);
    return sr;
}

ShapeRect shape_rect_from_vector4d(Vector4D v)
{
    ShapeRect sr;
    gfc_rect_set(sr,v.x,v.y,v.z,v.w);
    return sr;
}

SDL_Rect shape_rect_to_sdl_rect(ShapeRect r)
{
	SDL_Rect tmp = {r.x,r.y,r.w,r.h};
    return tmp;
}

/**
 * @brief check if the two circles have overlap
 * @param a the first circle to check
 * @param b the second circle to check
 * @return true if there is overlap, false if there is not
 */
Bool shape_circle_collision(ShapeCircle a, ShapeCircle b)
{
    float radii2;
    float c2;
    radii2 = (a.r*a.r) + (b.r * b.r);
    c2 = ((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y));
    if (radii2 <= c2)return false;
    return true;
}

/**
 * @brief check if a given point is inside a given circle
 * @param point the point in question
 * @param c the circle in question
 * @return true if there is overlap, false if there is not
 */
Bool shape_point_in_circle(ShapePoint point,ShapeCircle c)
{
    return shape_circle_collision(shape_circle(point.x,point.y,0), c);
}

/**
 * @brief check if a given point is inside a given rectangle
 * @param point the point in question
 * @param rect the rectangle in question
 * @return true if there is overlap, false if there is not
 */
Bool shape_point_in_rect(ShapePoint point,ShapeRect rect)
{
    if ((point.x < rect.x)||(point.x > rect.x + rect.w) ||
        (point.y < rect.y)||(point.y > rect.y + rect.h))
    {
        return false;
    }
    return true;
}


/**
 * @brief check if the two rectangles have overlap
 * @param a the first rectangle to check
 * @param b the second rectangle to check
 * @return true if there is overlap, false if there is not
 */
Bool shape_rect_collision(ShapeRect a,ShapeRect b)
{
    if (((a.x + a.w) < b.x) || (a.x > (b.x + b.w)) ||
       ((a.y + a.h) < b.y) || (a.y > (b.y + b.h)))
    {
        return false;
    }
    return true;

}

Bool shape_rect_circle_collision(ShapeCircle a, ShapeRect b)
{
	ShapeRect newrect;
    newrect = shape_rect_from_vector4d(vector4d(b.x - a.r,b.y,b.w + a.r+ a.r,b.h));
    if (shape_point_in_rect(vector2d(a.x,a.y),newrect))return true;
   	newrect = shape_rect_from_vector4d(vector4d(b.x,b.y - a.r,b.w,b.h + a.r + a.r));
    if (shape_point_in_rect(vector2d(a.x,a.y),newrect))return true;
    
    if (shape_point_in_circle(vector2d(b.x,b.y),a))return true;
    if (shape_point_in_circle(vector2d(b.x+b.w,b.y),a))return true;
    if (shape_point_in_circle(vector2d(b.x,b.y+b.h),a))return true;
    if (shape_point_in_circle(vector2d(b.x+b.w,b.y+b.h),a))return true;
    return false;
	
	
}
/*eol@eof*/
