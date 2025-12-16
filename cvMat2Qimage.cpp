#include "cvMat2Qimage.h"
#include <QDebug>
QImage cvMat2QImage(const cv::Mat& mat) 
{
    if (mat.empty())
    {
        return QImage();
    }
    QImage image;
    switch (mat.type())
    {
        qDebug()<<mat.type();
    case CV_8UC1:
    {
        image = QImage((const uchar*)(mat.data),
            mat.cols, mat.rows, mat.step,
            QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_8UC2:
    {
        mat.convertTo(mat, CV_8UC1);
        image = QImage((const uchar*)(mat.data),
            mat.cols, mat.rows, mat.step,
            QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_8UC3:
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_8UC4:
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    case CV_32FC1:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar *pSrc = (const uchar*)normalize_mat.data;
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, normalize_mat.step, QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_32FC3:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX,-1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar *pSrc = (const uchar*)normalize_mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, normalize_mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_64FC1:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar *pSrc = (const uchar*)normalize_mat.data;
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, normalize_mat.step, QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_64FC3:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar *pSrc = (const uchar*)normalize_mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, normalize_mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_32SC1:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar *pSrc = (const uchar*)normalize_mat.data;
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, normalize_mat.step, QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_32SC3:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar *pSrc = (const uchar*)normalize_mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, normalize_mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_16SC1:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar *pSrc = (const uchar*)normalize_mat.data;
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, normalize_mat.step, QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_16SC3:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar *pSrc = (const uchar*)normalize_mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, normalize_mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_8SC1:
    {
        //Mat normalize_mat;
        //normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        mat.convertTo(mat, CV_8U);
        const uchar *pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_8SC3:
    {
        mat.convertTo(mat, CV_8U);
        const uchar *pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    default:
        //CV_8UC1
        mat.convertTo(mat, CV_8UC1);
        QImage image((const uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
       // return image.rgbSwapped();
        return image.copy();
        break;
    }
}


cv::Mat QImage2cvMat(QImage& image)
{
    cv::Mat mat;
    //qDebug() << image.format();
    switch (image.format())
    {
    case QImage::Format_ARGB32:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB32:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        //cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        //cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_Grayscale8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}
#if 0
cv::Mat cvMatInt2cvMatChar(const cv::Mat& TempImage)
{

    cv::Mat DscImage ;
    DscImage =Mat::zeros(TempImage.cols, TempImage.rows, CV_8UC1);
    //    GaussianBlur(BriImage, TempImage, Size(9, 9), 14, 14);

    double minv = 0.0, maxv = 0.0;
    double* minp = &minv;
    double* maxp = &maxv;
    minMaxIdx(TempImage, minp, maxp);  //取得像素值最大值和最小值

    //用指针访问像素，速度更快
    const ushort *p_img;
    uchar *p_dst;
    for (int i = 0; i < TempImage.rows; i++)
    {

        p_img = TempImage.ptr<ushort>(i);//获取每行首地址
        p_dst = DscImage.ptr<quint8>(i);
        for (int j = 0; j < TempImage.cols; ++j)
        {
            p_dst[j] = (p_img[j] - minv) / (maxv - minv) * 255.0;
        }
    }
    return DscImage;

}
#endif

#if 0
cv::Mat cvMatInt2cvMatChar(const cv::Mat& TempImage)
{

    cv::Mat DscImage ;
    DscImage =Mat::zeros(TempImage.cols, TempImage.rows, CV_8UC1);
    double minv = 0.0, maxv = 0.0;
    double* minp = &minv;
    double* maxp = &maxv;
    minMaxIdx(TempImage, minp, maxp);  //取得像素值最大值和最小值
    qDebug()<<"the image row:"<<TempImage.rows<<"  the image cols:"<<TempImage.cols;
    qDebug()<<"the image Maxval:"<<minv<<"  the image Minval:"<<maxv;
    for (int i = 0; i < TempImage.rows; i++) {
        for (int j = 0; j < TempImage.cols; j++) {
            DscImage.at<uchar>(i, j) = (TempImage.at<ushort>(i, j) - minv) / (maxv - minv) * 255.0;
        }
    }

    return DscImage;

}
#endif


#if 0
cv::Mat cvMatInt2cvMatChar(const cv::Mat& TempImage)
{

    cv::Mat DscImage ;
    DscImage =Mat::zeros(TempImage.cols, TempImage.rows, CV_8UC1);
    double minv = 0.0, maxv = 0.0;
    double* minp = &minv;
    double* maxp = &maxv;
    minMaxIdx(TempImage, minp, maxp);  //取得像素值最大值和最小值
 //   qDebug()<<"the image row:"<<TempImage.rows<<"  the image cols:"<<TempImage.cols;
 //   qDebug()<<"the image Maxval:"<<maxv<<"  the image Minval:"<<minv ;



    MatConstIterator_<ushort> it_in=TempImage.begin<ushort>();
    MatConstIterator_<ushort> itend_in=TempImage.end<ushort>();
    MatIterator_<uchar> it_out=DscImage.begin<uchar>();
    MatIterator_<uchar> itend_out=DscImage.end<uchar>();
    while(it_in!=itend_in)
    {
        //(*it_out)[0]=(*it_in)[0]/div*div+div/2;
        (*it_out)= (TempImage.at<ushort>() - minv) / (maxv - minv) * 255.0;
        it_in++;
        it_out++;
        if(it_out==itend_out)
            ;

    }




    return DscImage;

}
#endif

#if 0
cv::Mat cvMatInt2cvMatChar(const cv::Mat& TempImage)
{

    cv::Mat DscImage ;
    DscImage =Mat::zeros(TempImage.cols, TempImage.rows, CV_8UC1);
    CV_Assert(TempImage.depth()!=DscImage.depth());
    double minv = 0.0, maxv = 0.0;
    double* minp = &minv;
    double* maxp = &maxv;
    minMaxIdx(TempImage, minp, maxp);  //取得像素值最大值和最小值

    MatConstIterator_<ushort> it_in=TempImage.begin<ushort>();
    MatConstIterator_<ushort> itend_in=TempImage.end<ushort>();
    MatIterator_<uchar> it_out=DscImage.begin<uchar>();
    MatIterator_<uchar> itend_out=DscImage.end<uchar>();
    while(it_in!=itend_in)
    {
        //(*it_out)[0]=(*it_in)[0]/div*div+div/2;
        (*it_out)= (TempImage.at<ushort>() - minv) / (maxv - minv) * 255.0;
        it_in++;
        it_out++;
        if(it_out==itend_out)
            ;

    }




    return DscImage;

}
#endif

#if 1
cv::Mat cvMatInt2cvMatChar(const cv::Mat& TempImage)
{
    cv::Mat DscImage ;
    DscImage =Mat::zeros(TempImage.rows, TempImage.cols, CV_8UC1);
    CV_Assert(TempImage.depth()!=DscImage.depth());
    double minv = 0.0, maxv = 0.0;
    double* minp = &minv;
    double* maxp = &maxv;
    minMaxIdx(TempImage, minp, maxp);  //取得像素值最大值和最小值
    for(int i =0;i<TempImage.rows;i++)
    {
        for(int j=0;j<TempImage.cols;j++)
        {
            DscImage.at<uchar>(i,j)= (TempImage.at<ushort>(i,j) - minv) / (maxv - minv) * 255.0;

        }
    }
 //   qDebug()<<"the max val"<<maxv<<"the min val"<<minv;
    // switch(TempImage.channels())
    // {
    // case 1:
    //     for(int i =0;i<TempImage.rows;i++)
    //     {
    //         for(int j=0;j<TempImage.cols;j++)
    //         {
    //             DscImage.at<uchar>(i,j)= (TempImage.at<ushort>(i,j) - minv) / (maxv - minv) * 255.0;

    //         }
    //     }
    //     break;
    // case 2:
    //     for(int i =0;i<TempImage.rows;i++)
    //     {
    //         for(int j=0;j<TempImage.cols;j++)
    //         {
    //             DscImage.at<uchar>(i,j)= (TempImage.at<ushort>(i,j) - minv) / (maxv - minv) * 255.0;
    //         //    DscImage.at<Vec2b>(i,j)[1]= (TempImage.at<ushort>(i,j)[2] - minv) / (maxv - minv) * 255.0;

    //         }
    //     }
    //     break;
    // case 3:
    //     for(int i =0;i<TempImage.rows;i++)
    //     {
    //         for(int j=0;j<TempImage.cols;j++)
    //         {
    //             DscImage.at<uchar>(i,j)= (TempImage.at<ushort>(i,j) - minv) / (maxv - minv) * 255.0;

    //         }
    //     }
    //     break;
    // case 4:
    //     for(int i =0;i<TempImage.rows;i++)
    //     {
    //         for(int j=0;j<TempImage.cols;j++)
    //         {
    //             DscImage.at<uchar>(i,j)= (TempImage.at<ushort>(i,j) - minv) / (maxv - minv) * 255.0;

    //         }
    //     }
    //     break;
    // }
    return DscImage;
}
#endif

cv::Mat intMat2Char(cv::Mat& TempImage,int value)
{
    if(value>=16)
        value=16;
    if(value<4)
        value=4;
    cv::Mat DscImage;
    DscImage.zeros(TempImage.rows,TempImage.cols,CV_8UC1);

    if(value>=8)
    {
        int    diffValue=value-8;
        for(int i=0;i<TempImage.rows;i++)
        {
            for(int j=0;j<TempImage.cols;j++)
            {
                DscImage.at<char>(i,j)=TempImage.at<ushort>(i,j)>>diffValue;
            }
        }
    }
    else
    {
        int    diffValue=8-value;
        for(int i=0;i<TempImage.rows;i++)
        {
            for(int j=0;j<TempImage.cols;j++)
            {
                DscImage.at<char>(i,j)=TempImage.at<ushort>(i,j)<<diffValue;
            }
        }
    }

    return DscImage;
}
