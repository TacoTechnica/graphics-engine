
//#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>

#include "renderer.h"
#include "edgebuffer.h"

Renderer::Renderer(Image *img) {
    this->img = img;
    zbuffer = new Matrix(img->getWidth(), img->getHeight());
    clearZBuffer();
}

/** plot(x,y)
 *
 *      Plots a single pixel on our image
 */
void Renderer::plot(int x, int y, float z) {
    // Flip the y axis
    y = getImage()->getHeight() - y;
    // If we're within the image
    if (x >= 0 && y >= 0 && x < img->getWidth() && y < img->getHeight()) {
        // If we're above the z value on this pixel, replace this pixel
        if (*zbuffer->get(x, y) <= z) {
            memcpy(img->getPixel(x,y), &color, sizeof(struct Color));
            *zbuffer->get(x, y) = z;
        }
    }
}

/** refill()
 *
 *      Refills the screen completely with our draw color
 */
void Renderer::refill() {
    int xx, yy;
    for(yy = 0; yy < img->getHeight(); yy++) {
        for(xx = 0; xx < img->getWidth(); xx++) {
            plot(xx,yy, FLT_MAX);
        }
    }
    clearZBuffer();
}

/* drawLine(x0, y0, x1, y1)
 *
 *      Draws a line from (x0, y0) to (x1, y1) on our image
 */
void Renderer::drawLine(int x0, int y0, float z0, int x1, int y1, float z1) {

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

    float dz = z1 - z0;

    if (dy >= 0) { // Quadrant 1 and 3
        // Octant 1 and 5
        if (dy <= dx) {
            d = 2*a + b;
            while(x0 <= x1) {
                float zNow = z0 + dz * (float)x0 / (float)dx;
                plot(x0, y0, zNow);
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
                float zNow = z0 + dz * (float)y0 / (float)dy;
                plot(x0, y0, zNow);
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
                float zNow = z0 + dz * (float)x0 / (float)dx;
                plot(x0, y0, zNow);
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
                float zNow = z0 + dz * (float)y0 / (float)dy;
                plot(x0, y0, zNow);
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

void Renderer::fillRect(int x, int y, int w, int h) {
    int xx, yy;
    for(xx = x; xx < x+w; xx++) {
        for(yy = y; yy < y+h; yy++) {
            plot(xx,yy, -FLT_MAX);
        }
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
            *mat->get(col, 2),
            *mat->get(col+1, 0),
            *mat->get(col+1, 1),
            *mat->get(col+1, 2)
        );
    }
}

void Renderer::drawTriangleBufferMesh(TriangleBuffer *buffer) {


    Matrix *mat = buffer->getPoints();
    int col;
    for(col = 0; col < mat->getNumColumns(); col+=3) {
        if (col >= mat->getNumColumns() || col+2 >= mat->getNumColumns())
            break;

        //struct Color p = {100, 100, 100};
        struct Color p = {rand(), rand(), rand()};
        setColor(p);

        Vector3f *p0 = mat->getColumnVector(col);
        Vector3f *p1 = mat->getColumnVector(col + 1);
        Vector3f *p2 = mat->getColumnVector(col + 2);

        Vector3f *d1 = Vector3f::getDelta(p0, p1);
        Vector3f *d2 = Vector3f::getDelta(p0, p2);

        Vector3f *normal = Vector3f::getCrossProduct(d1, d2);
        Vector3f *view = new Vector3f(0, 0, 1);
        float normalDotView = Vector3f::getDotProduct(normal, view);

        if (normalDotView <= 0) {
            //p = {255, 0, 0};
            //struct Color p = {rand(), rand(), rand()};
            //setColor(p);
            continue; // Skip this triangle
        }


        // SCANLINE

        // Pick top, bottom and middle points
        Vector3f *ptop, *pmid, *pbot;  // y
        if (       p0->getY() > p1->getY() && p0->getY() > p2->getY()) {
            ptop = p0;
            if (p1->getY() > p2->getY()) {
                pmid = p1;
                pbot = p2;
            } else {
                pmid = p2;
                pbot = p1;
            }
        } else if (p1->getY() > p0->getY() && p1->getY() > p2->getY()) {
            ptop = p1;
            if (p2->getY() > p0->getY()) {
                pmid = p2;
                pbot = p0;
            } else {
                pmid = p0;
                pbot = p2;
            }
        } else {
            ptop = p2;
            if (p1->getY() > p0->getY()) {
                pmid = p1;
                pbot = p0;
            } else {
                pmid = p0;
                pbot = p1;
            }
        }

        int currentY;
        float xStart = pbot->getX();
        float yStart = pbot->getY();
        float zStart = pbot->getZ();

        // Left side of triangle, delta
        float dxLeft = ptop->getX() - pbot->getX();
        float dyLeft = ptop->getY() - pbot->getY();
        float dzLeft = ptop->getZ() - pbot->getZ();

        // Right side of triangle, delta
        float dxRight = pmid->getX() - pbot->getX();
        float dyRight = pmid->getY() - pbot->getY();
        float dzRight = pmid->getZ() - pbot->getZ();

        for(currentY = 0; currentY < dyRight; currentY++) {
            float yy = (float)currentY;
            float xleft = (dxLeft / dyLeft) * yy;
            float xright = (dxRight / dyRight) * yy;
            float zleft = (dzLeft / dyLeft) * yy;
            float zright = (dzRight / dyRight) * yy;

            xleft +=  xStart;
            zleft +=  zStart;
            xright += xStart;
            zright += zStart;
            yy     += yStart;
            drawLine(xleft,  yy, zleft,
                     xright, yy, zright);
        }

        dxRight = ptop->getX() - pmid->getX();
        dyRight = ptop->getY() - pmid->getY();
        dzRight = ptop->getZ() - pmid->getZ();

        // TODO: Put me in a repeating loop
        for(currentY = 0; currentY < dyRight; currentY++) {
            float yyLeft = (float)currentY + (pmid->getY() - yStart);
            float yyRight = (float)currentY;

            float xleft = (dxLeft / dyLeft) * yyLeft;
            float xright = (dxRight / dyRight) * yyRight;
            float zleft = (dzLeft / dyLeft) * yyLeft;
            float zright = (dzRight / dyRight) * yyRight;

            xleft +=  xStart;
            zleft +=  zStart;
            xright += pmid->getX();
            zright += pmid->getZ();
            float yy = yyLeft + yStart;
            drawLine(xleft,  yy, zleft,
                     xright, yy, zright);
        }

        // Draw the line
        p = {0, 0, 0};
        setColor(p);

        drawLine( 
            p0->getX(),
            p0->getY(),
            p0->getZ(),
            p1->getX(),
            p1->getY(),
            p1->getZ()
            //*mat->get(col, 0),
            //*mat->get(col, 1),
            //*mat->get(col+1, 0),
            //*mat->get(col+1, 1)
        );
        drawLine(
            p1->getX(),
            p1->getY(),
            p1->getZ(),
            p2->getX(),
            p2->getY(),
            p2->getZ()
            //*mat->get(col+1, 0),
            //*mat->get(col+1, 1),
            //*mat->get(col+2, 0),
            //*mat->get(col+2, 1)
        );
        drawLine(
            p2->getX(),
            p2->getY(),
            p2->getZ(),
            p0->getX(),
            p0->getY(),
            p0->getZ()
            //*mat->get(col+2, 0),
            //*mat->get(col+2, 1),
            //*mat->get(col, 0),
            //*mat->get(col, 1)
        );

        //bool switched = false;
        //for(currentY = 0; !(currentY >= dyRight && switched); currentY++) {
        //    // Offset left side if we're switched
        //    float xleft =  (dxLeft  / dyLeft ) * (currentY + switched? dyRight : 0); // TODO: Redo this
        //    float xright = (dxRight / dyRight) * currentY;
        //    float zleft =  (dzLeft  / dyLeft)  * (currentY + switched? dyRight : 0);
        //    float zright = (dzRight / dyRight) * currentY;
        //    
        //    xleft += xStart;
        //    zleft += zStart;
        //    xright += switched? xStart : pmiddle->getX();
        //    zright += switched? zStart : pmiddle->getZ();

        //    drawLine(xleft,  currentY + switched? dyRight : 0, zleft,
        //             xright, currentY + switched? dyRight : 0, zright);

        //    // Switch after reaching the middle!
        //    if (currentY >= dyRight) {
        //        if (switched) break;
        //        switched = true;

        //        currentY = 0;

        //        dxRight = ptop->getX() - pmid->getX();
        //        dyRight = ptop->getY() - pmid->getY();
        //        dzRight = ptop->getZ() - pmid->getZ();
        //    }
        //}

    }

}

void Renderer::clearZBuffer() {
    int xx, yy;
    for(xx = 0; xx < zbuffer->getNumColumns(); xx++) {
        for(yy = 0; yy < zbuffer->getNumRows(); yy++) {
            *zbuffer->get(xx,yy) = -1 * FLT_MAX;
        }
    }
}
