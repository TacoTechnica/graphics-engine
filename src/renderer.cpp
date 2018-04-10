
//#include<stdio.h>
#include<string.h>

#include "renderer.h"
#include "edgebuffer.h"

/** plot(x,y)
 *
 *      Plots a single pixel on our image
 */
void Renderer::plot(int x, int y) {
    y = getImage()->getHeight() - y;
    if (x >= 0 && y >= 0 && x < img->getWidth() && y < img->getHeight())
        memcpy(img->getPixel(x,y), &color, sizeof(struct Color));
}

/** refill()
 *
 *      Refills the screen completely with our draw color
 */
void Renderer::refill() {
    int xx, yy;
    for(yy = 0; yy < img->getHeight(); yy++) {
        for(xx = 0; xx < img->getWidth(); xx++) {
            plot(xx,yy);
        }
    }
}

/* drawLine(x0, y0, x1, y1)
 *
 *      Draws a line from (x0, y0) to (x1, y1) on our image
 */
void Renderer::drawLine(int x0, int y0, int x1, int y1) {
    // swapping
    int swap;
    if (x0 > x1) {
        swap = x0;
        x0 = x1;
        x1 = swap;
        swap = y0;
        y0 = y1;
        y1 = swap;
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int a = dy; // TODO: You can remove a and b...
    int b = -dx;
    int d;

    if (dy >= 0) { // Quadrant 1 and 3
        // Octant 1 and 5
        if (dy <= dx) {
            d = 2*a + b;
            while(x0 <= x1) {
                plot(x0, y0);
                if (d > 0) {
                    y0++;
                    d += 2*b;
                }
                x0++;
                d += 2*a;
            }
        // Octant 2 and 6
        } else {
            d = a + 2*b;
            while(y0 <= y1) {
                plot(x0, y0);
                if (d < 0) {
                    x0++;
                    d += 2*a;
                }
                y0++;
                d += 2*b;
            }
            
        }
        return;
    } else { // Quadrant 2 and 4 
        // Octant 4 and 8
        if (-dy <= dx) {
            d = -2*a + b;
            while(x0 <= x1) {
                plot(x0, y0);
                if (d > 0) {
                    y0--;
                    d += 2*b;
                }
                x0++;
                d -= 2*a;
            }
        // Octant 2 and 6
        } else {
            d = -a + 2*b;
            while(y0 >= y1) {
                plot(x0, y0);
                if (d < 0) {
                    x0++;
                    d -= 2*a;
                }
                y0--;
                d += 2*b;
            }
        }
        return;
    }
}

void Renderer::drawEdgeBufferLines(EdgeBuffer *buffer) {
    Matrix *mat = buffer->getPoints();
    int col;
    for(col = 0; col < mat->getNumColumns(); col+=2) {
        if (col >= mat->getNumColumns() || col+1 >= mat->getNumColumns())
            break;

        drawLine( 
            *mat->get(col, 0),
            *mat->get(col, 1),
            *mat->get(col+1, 0),
            *mat->get(col+1, 1)
        );
    }
}

void Renderer::drawTriangleBufferMesh(TriangleBuffer *buffer) {
    Matrix *mat = buffer->getPoints();
    int col;
    for(col = 0; col < mat->getNumColumns(); col+=3) {
        if (col >= mat->getNumColumns() || col+2 >= mat->getNumColumns())
            break;

        Vector3f *p0 = mat->getColumnVector(col);
        Vector3f *p1 = mat->getColumnVector(col + 1);
        Vector3f *p2 = mat->getColumnVector(col + 2);

        Vector3f *d1 = Vector3f::getDelta(p0, p1);
        Vector3f *d2 = Vector3f::getDelta(p0, p2);

        Vector3f *normal = Vector3f::getCrossProduct(d1, d2);
        Vector3f *view = new Vector3f(0, 0, 1);
        float normalDotView = Vector3f::getDotProduct(normal, view);

        if (normalDotView <= 0) {
            continue; // Skip this triangle
        }

        // TODO: Replace these calls with vector calls
        drawLine( 
            p0->getX(),
            p0->getY(),
            p1->getX(),
            p1->getY()
            //*mat->get(col, 0),
            //*mat->get(col, 1),
            //*mat->get(col+1, 0),
            //*mat->get(col+1, 1)
        );
        drawLine(
            p1->getX(),
            p1->getY(),
            p2->getX(),
            p2->getY()
            //*mat->get(col+1, 0),
            //*mat->get(col+1, 1),
            //*mat->get(col+2, 0),
            //*mat->get(col+2, 1)
        );
        drawLine(
            p2->getX(),
            p2->getY(),
            p0->getX(),
            p0->getY()
            //*mat->get(col+2, 0),
            //*mat->get(col+2, 1),
            //*mat->get(col, 0),
            //*mat->get(col, 1)
        );
    }

}
