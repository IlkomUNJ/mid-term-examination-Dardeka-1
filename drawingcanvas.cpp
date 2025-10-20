#include "drawingcanvas.h"

DrawingCanvas::DrawingCanvas(QWidget *parent)  {
    // Set a minimum size for the canvas
    setMinimumSize(this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
    // Set a solid background color
    setStyleSheet("background-color: white; border: 1px solid gray;");
}

void DrawingCanvas::clearPoints(){
    m_points.clear();
    // Trigger a repaint to clear the canvas
    update();
}

void DrawingCanvas::paintLines(){
    /* Todo
     * Implement lines drawing per even pair
    */

    isPaintLinesClicked = true;
    update();
}

void DrawingCanvas::segmentDetection(){
    QPixmap pixmap = this->grab(); //
    QImage image = pixmap.toImage();

    cout << "image width " << image.width() << endl;
    cout << "image height " << image.height() << endl;

    //To not crash we set initial size of the matrix
    vector<CustomMatrix> windows(image.width()*image.height());

    // // For Matrix 3 x 3
    // // Get the pixel value as an ARGB integer (QRgb is a typedef for unsigned int)
    // int count_true_mat3 = 0;
    // for(int i = 1; i < image.width()-1;i++){
    //     for(int j = 1; j < image.height()-1;j++){
    //         bool local_window[3][3] = {false};

    //         for(int m=-1;m<=1;m++){
    //             for(int n=-1;n<=1;n++){
    //                 QRgb rgbValue = image.pixel(i+m, j+n);
    //                 local_window[m+1][n+1] = (rgbValue != 0xffffffff);
    //                 if(local_window[m+1][n+1] == 1){
    //                     cout << "\n The matrix in local_window ("<<m+1<<","<< n+1 << ") is " << local_window[m+1][n+1] << endl;
    //                     count_true_mat3++;
    //                     break;
    //                 }
    //             }
    //         }

    //         CustomMatrix mat3(local_window);

    //         windows.push_back(mat3);
    //     }
    // }
    // cout << "\n Total color detected in 3 x 3 = "<<count_true_mat3;

    // For Matrix 5 x 5
    // Use sub-matrix for better detection
    int count_true_mat5 = 0;
    for(int a = 2; a < image.width()-2; a++){
        for(int b = 2; b < image.height()-2; b++){
            bool local_window_mat5[5][5] = {false};

            for(int c = -2; c <=2; c++){
                for(int d = -2; d<=2; d++){
                    QRgb colorVal = image.pixel(a+c, b+d);
                    local_window_mat5[c+2][d+2] = (colorVal != 0xffffffff);
                    if(local_window_mat5[c+2][d+2] == 1){
                        cout << "\n The matrix in local_window ("<<c+2<< ","<< d+2 << ") is " << local_window_mat5[c+2][d+2] << endl;
                        count_true_mat5++;
                    }
                }
            }

            CustomMatrix mat5(local_window_mat5);

            windows.push_back(mat5);
        }
    }
    cout << "Total color detected in 5 x 5 = "<<count_true_mat5;

    return;
}

void DrawingCanvas::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set up the pen and brush for drawing the points
    QPen pen(Qt::blue, 5);
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::blue));

    // Draw a small circle at each stored point
    for (const QPoint& point : std::as_const(m_points)) {
        painter.drawEllipse(point, 3, 3);
    }

    if(isPaintLinesClicked){
        cout << "paint lines block is called" << endl;
        pen.setColor(Qt::red);
        pen.setWidth(4); // 4-pixel wide line
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);

        // Set the painter's pen to our custom pen.
        painter.setPen(pen);

        for(int i=0;i<m_points.size()-1;i+=2){
            //cout << m_points[i].x() << endl;
            painter.drawLine(m_points[i], m_points[i+1]);
        }
        isPaintLinesClicked = false;

        //return painter pen to blue
        pen.setColor(Qt::blue);
        painter.setPen(pen);
    }
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event) {
    // Add the mouse click position to our vector of points
    m_points.append(event->pos());
    // Trigger a repaint
    update();
}


