#include "stdafx.h"
#include "ImageProcessing.h"
#include "Main.h"
#include <cmath>

namespace ImageProcessing
{
	vector<ConnectedElements> ImageAnalization::FindConnectedParts(const Mat& inputImg) const
	{
		//http://nghiaho.com/uploads/code/opencv_connected_component/blob.cpp

		vector<ConnectedElements> blobs;
		blobs.clear();

		// Fill the label_image with the blobs
		// 0  - background
		// 1  - unlabeled foreground
		// 2+ - labeled foreground

		Mat labelImage;
		inputImg.convertTo(labelImage, CV_32SC1);

		Rect labelImageCenter(labelImage.cols / 3, labelImage.rows / 3, labelImage.cols / 3, labelImage.rows / 3);

		int labelCount = 2; // starts at 2 because 0,1 are used already

		for (int y = 0; y < labelImage.rows; ++y)
		{
			const auto row = (int*)labelImage.ptr(y);
			for (int x = 0; x < labelImage.cols; ++x)
			{
				if (row[x] > 0)
				{
					continue;
				}

				Rect rect;
				floodFill(labelImage, Point(x, y), labelCount, &rect, 0, 0, 4);

				vector<Point2i> blob;

				int val = 0;
				int ptInsideCenter = 0;
				for (int yRoi = rect.y; yRoi < (rect.y + rect.height); ++yRoi)
				{
					const auto row2 = (int*)labelImage.ptr(yRoi);
					for (int xRoi = rect.x; xRoi < (rect.x + rect.width); ++xRoi)
					{
						if (row2[xRoi] != labelCount)
							continue;
						if (xRoi == 0 || yRoi == 0)
							continue;

						int multiplX = labelImage.cols % xRoi;
						int multiplY = labelImage.rows % yRoi;
						if (xRoi > labelImage.cols / 2)
						{
							multiplX = labelImage.cols - xRoi;
						}
						if (yRoi > labelImage.rows / 2)
						{
							multiplY = labelImage.rows - yRoi;
						}

						val += xRoi * multiplX * multiplY;

						if (labelImageCenter.contains(Point(xRoi, yRoi)))
						{
							ptInsideCenter++;
						}

						blob.emplace_back(xRoi, yRoi);
					}
				}

				blobs.push_back(ConnectedElements(blob, rect, val, ptInsideCenter));

				labelCount++;
			}
		}

		return blobs;
	}

	tuple<string, int> ImageAnalization::RunTesseract(Mat img) const
	{
		// https://github.com/tesseract-ocr/tesseract/wiki/APIExample
		// https://stackoverflow.com/questions/8115368/converting-cvmat-for-tesseract
		_tessApi->SetImage((uchar*)img.data, img.size().width, img.size().height, img.channels(), (int)img.step1());

		// Get OCR result
		char* outText = _tessApi->GetUTF8Text(); // don't forget to delete the outText Pointer
		int* confidenceArray = _tessApi->AllWordConfidences();
		int confidence = 0;
		if (confidenceArray != nullptr)
			confidence = confidenceArray[0];

		string str = string(outText);
		delete[] outText;
		delete[] confidenceArray;

		str.erase(remove(str.begin(), str.end(), '\n'), str.end());

		return {str, confidence};
	}

	void ImageAnalization::SaveImage(const Mat& img, const string& recognizedValue, const int confidence) const
	{
		SYSTEMTIME st;
		GetSystemTime(&st);
		char str[MAX_PATH];
		sprintf_s(str, MAX_PATH, "%s/%s/%04d-%02d-%02d %02d-%02d-%03d_%03d.jpg",DATASET_PATH, recognizedValue.c_str(),
				  st.wYear,
				  st.wMonth,
				  st.wDay, st.wMinute, st.wSecond, st.wMilliseconds, confidence);
		imwrite(str, img);
	}

	void ImageAnalization::EvaluateNumberSegments()
	{
		_sudoku.clear();
		for (int img = 0; img < 9 * 9; ++img)
		{
			Mat* singleNumberImg = &_singleImages[img]._Img;

			const bool useConnectedElements = true;
			if (useConnectedElements)
			{
				auto blobs = FindConnectedParts(*singleNumberImg);

				sort(blobs.begin(), blobs.end(), ConnectedElements::Comparer());

				*singleNumberImg = Mat(singleNumberImg->rows, singleNumberImg->cols, CV_8UC1, 255);


				if (blobs.empty() || blobs[0]._PtInsideCenter == 0)
				{
					// no connected elements found in picture, add 0 to sudoku
					_sudoku.push_back(0);
					continue;
				}

				for (auto& point : blobs[0]._Points)
				{
					const int x = point.x;
					const int y = point.y;

					singleNumberImg->at<uchar>(y, x) = 0;
				}
			}

			// Tesseract _Number recognition
			auto tessResult = RunTesseract(*singleNumberImg);
			const string tessText = std::get<0>(tessResult);
			const int tessConfidence = std::get<1>(tessResult);

#if STOREDATASET == 1
			if (tessConfidence > 50)
				SaveImage(*singleNumberImg, tessText, tessConfidence);
#endif
			char str[200];
			sprintf_s(str, 200, "OCR %dx%d (%2d): \"%s\" (%2d%s)", img / 9 + 1, img % 9 + 1, img, tessText.c_str(),
					  tessConfidence, "%");

			cout << str << endl;

			putText(*singleNumberImg, tessText, Point(0, singleNumberImg->rows), FONT_HERSHEY_SIMPLEX, 1, 0, 1);

			_sudoku.push_back(atoi(tessText.c_str()));
		}
		SetEndTime();
	}

	void ImageAnalization::SegmentSingleNumbers(Mat img)
	{
		const int cols = img.cols;
		const int rows = img.rows;


		const int padding = -3;

		if (padding < 0)
			copyMakeBorder(img, img, abs(padding), abs(padding), abs(padding), abs(padding), BORDER_CONSTANT, 255);

		// this variable is created to avoid the situation where the help rectangle
		// (drawn below) influences the evaluated image
		const Mat tmpImg = img.clone();

		const int widthSingleCell = cols / 9;
		const int heightSingelCell = rows / 9;

		for (int y = 0; y < 9; y++)
		{
			for (int x = 0; x < 9; x++)
			{
				int posX = x * widthSingleCell + (padding < 0 ? abs(padding) : 0);
				int posY = y * heightSingelCell + (padding < 0 ? abs(padding) : 0);

				const Point pt1 = Point(posX + padding, posY + padding);

				posX += widthSingleCell;
				posY += heightSingelCell;

				const Point pt2 = Point(posX - padding, posY - padding);

				// Rectangle only for visualization
				rectangle(img, pt1, pt2, Scalar(0, 0, 0)); // draw rect on original image

				// Segment images to sudoku field
				const Rect roi = Rect(pt1, pt2);
				_singleImages[y * 9 + x] = SingleImage(tmpImg(roi), roi); // take clone of image without help rectangle
			}
		}

		_operationImages[RectangleInformation] = img;

		EvaluateNumberSegments();
	}

	void ImageAnalization::SortRoiPoints(Point2f input[4])
	{
		Point2f sortedCpy[4];
		copy(input, input+4, sortedCpy);
		// sort whole array according to y
		// take first two elements (= the upper two points) and sort them according to x distance
		// this way we get the first point (top left)

		sort(begin(sortedCpy), end(sortedCpy), [](const Point2f comp1, const Point2f comp2)
		{
			return comp1.y < comp2.y;
		});

		sort(begin(sortedCpy), begin(sortedCpy) + 2, [](const Point2f comp1, const Point2f comp2)
		{
			return comp1.x < comp2.x;
		});

		// turn roi until points fit required position
		while (sortedCpy[0] != input[0])
		{
			auto first = input[0];
			for (int i = 0; i < 3; ++i)
				input[i] = input[i + 1];
			input[3] = first;
		}

		// if roi was defined counter clockwise make it clockwise
		if (sortedCpy[1] != input[1])
		{
			iter_swap(input + 1, input + 3);
		}
	}

	void ImageAnalization::CropImageToRoi(Mat img)
	{
		//http://opencvexamples.blogspot.com/2014/01/perspective-transform.html

		// The 4 points that select quadilateral on the input , from top-left in clockwise order
		// These four pts are the sides of the rect box used as input
		//	_inputQuad[i]

		SortRoiPoints(_inputQuad);
		// The 4 points where the mapping is to be done , from top-left in clockwise order
		_outputQuad[0] = Point2f(0, 0);
		_outputQuad[1] = Point2f((float)img.cols - 1, 0);
		_outputQuad[2] = Point2f((float)img.cols - 1, (float)img.rows - 1);
		_outputQuad[3] = Point2f(0, (float)img.rows - 1);

		// Get the Perspective Transform Matrix i.e. transofrmMatrix
		const Mat transformMatrix = getPerspectiveTransform(_inputQuad, _outputQuad);
		Mat warped;
		// Apply the Perspective Transform just found to the src image
		warpPerspective(img, warped, transformMatrix, warped.size(), CV_INTER_AREA);

		Mat binary;
		_operationImages[Warped] = warped;

		// Convert color to gray scale
		cvtColor(warped, binary, COLOR_RGBA2GRAY);

		Scalar average = mean(binary);

		const int thresholdValue = (int)round(average[0] - average[0] / 3);
		// Convert to 0 - 255
		threshold(binary, binary, thresholdValue, 255, THRESH_BINARY);

		_operationImages[Binarized] = binary;

		Mat morph = binary.clone();

		const bool withMorphology = false;
		if (withMorphology)
		{
			int morphSize = 1;
			Mat kernel = getStructuringElement(MorphShapes::MORPH_CROSS, Size(2 * morphSize + 1, 2 * morphSize + 1),
											   Point(-morphSize, -morphSize));
			morphologyEx(morph, morph, MorphTypes::MORPH_CLOSE, kernel, Point(-1, -1));

			_operationImages[MorphologicalOperation] = morph;
		}
		SegmentSingleNumbers(morph.clone());
	}

	void ImageAnalization::DrawRoiOnImage(const Mat& img, int pos) const
	{
		if (pos < 1)
			return;

		const Point pt[1][4] = {
			(Point)_inputQuad[0],
			(Point)_inputQuad[1],
			(Point)_inputQuad[2],
			(Point)_inputQuad[3]
		};

		const Point* elementPoints[1] = {pt[0]};

		int numberOfPoints = pos;

		Mat overlay;

		img.copyTo(overlay);

		fillPoly(overlay, elementPoints, &numberOfPoints, 1, Scalar(77, 84, 98));

		const double alpha = 0.5;
		addWeighted(overlay, alpha, img, 1 - alpha, 0, overlay);

		imshow("Sudoku Window", overlay);
	}

	void CallBackFunc(int event, int x, int y, int flags, void* userdata)
	{
		auto classInst = (ImageAnalization *)userdata;
		Mat* img = classInst->GetSourceImage();

		if (event == EVENT_RBUTTONDOWN)
		{
			classInst->_roiPos = 0;
			cout << "Reset roi" << endl;
		}
		else if (event == EVENT_LBUTTONDOWN)
		{
			cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;

			if (classInst->_roiPos < 4)
			{
				classInst->_inputQuad[classInst->_roiPos].x = float(x);
				classInst->_inputQuad[classInst->_roiPos].y = float(y);
			}
			classInst->_roiPos++; // gets retested below

			if (classInst->_roiPos > 3)
			{
				cout << "Start evaluaion" << endl;
				for (int i = 0; i < 4; ++i)
				{
					cout << classInst->_inputQuad[i].x << ", " << classInst->_inputQuad[i].y << "," << endl;
				}
				classInst->SetBeginTime();
				classInst->CropImageToRoi(*img);
			}
		}
		else if (event == EVENT_MOUSEMOVE)
		{
			if (classInst->_roiPos > 3)
				return;
			classInst->_inputQuad[classInst->_roiPos].x = float(x);
			classInst->_inputQuad[classInst->_roiPos].y = float(y);
			classInst->DrawRoiOnImage(*img, classInst->_roiPos + 1);
		}
	}

	void OnTrackBarNumberChanged(int sliderValue, void* userData)
	{
		if (sliderValue < 0 || sliderValue >= 9 * 9)
			return;

		auto classInst = (ImageAnalization *)userData;
		auto singleImage = *classInst->GetSingleImage(sliderValue);

		if (singleImage._Img.empty())
			return;

		const int width = max(singleImage._Img.cols, 300);
		const int height = max(singleImage._Img.rows, 300);

		imshow("Number", singleImage._Img);
		resizeWindow("Number", width, height);
	}

	void OnTrackbarOperationImagesChanged(int sliderValue, void* userData)
	{
		if (sliderValue < 0 || sliderValue >= ImageAnalization::NumOperationImages)
			return;

		auto classInst = (ImageAnalization *)userData;
		auto operationImagae = *classInst->GetOperationImage(sliderValue);

		if (operationImagae.empty())
			return;

		const int width = max(operationImagae.cols, 300);
		const int height = max(operationImagae.rows, 300);

		imshow("OperationImage", operationImagae);
		resizeWindow("OperationImage", width, height);
	}

	void ImageAnalization::ProcessImage(char* path)
	{
		_tessApi = new tesseract::TessBaseAPI();
		// Initialize tesseract-ocr with English, without specifying tessdata path
		if (_tessApi->Init("./tessdata", "eng"))
		{
			std::cout << "Cannot Initialize Tesseract" << endl;
			return;
		}
		_tessApi->SetVariable("tessedit_char_whitelist", "123456789");
		_tessApi->SetPageSegMode(tesseract::PSM_SINGLE_CHAR);

		_sourceImage = imread(path, CV_LOAD_IMAGE_COLOR);

		const int maxSize = 1000;
		const double rows = _sourceImage.rows;
		const double cols = _sourceImage.cols;
		if (cols > maxSize || rows > maxSize)
		{
			const double ratio = min(maxSize / rows, maxSize / cols);
			resize(_sourceImage, _sourceImage, Size(0, 0), ratio, ratio, CV_INTER_AREA);
		}


#ifdef _DEBUG
#ifdef PRESET_ROI
#if USE_PRESET_ROI == 1
		int pos[] = PRESET_ROI;
		SetDebugPosition(pos);
		SetBeginTime();
		CropImageToRoi(_sourceImage);
		destroyAllWindows();

		// Destroy used tesseract object and release memory
		_tessApi->End();
		return;
#endif
#endif
#endif

#ifdef _DEBUG
		namedWindow("OperationImage", WINDOW_AUTOSIZE);
		createTrackbar("Operation images", "OperationImage", 0, NumOperationImages - 1,
					   OnTrackbarOperationImagesChanged, this);

		namedWindow("Number", WINDOW_AUTOSIZE);
		createTrackbar("Image Number", "Number", 0, 9 * 9 - 1, OnTrackBarNumberChanged, this);
#endif

		namedWindow("Sudoku Window", WINDOW_AUTOSIZE);
		//set the callback function for any mouse event
		setMouseCallback("Sudoku Window", CallBackFunc, (void*)this);

		imshow("Sudoku Window", _sourceImage);

		waitKeyEx(0);

		destroyAllWindows();

		// Destroy used tesseract object and release memory
		_tessApi->End();
	}

	void ImageAnalization::DrawSolution(vector<int> solution)
	{
		if (_sudoku.empty())
			throw out_of_range("_sudoku string is empty");

		namedWindow("Solution", WINDOW_AUTOSIZE);

		Mat warped;
		Mat solText = Mat(_operationImages[Binarized].rows, _operationImages[Binarized].cols, CV_8UC3, Scalar(0, 0, 0));

		// Cycle over sudoku solution and if _Number is not one of the given numbers draw on picture
		for (auto i = 0; i < solution.size(); i++)
		{
			if (_sudoku[i] == 0) // _Number is not given by image
			{
				const SingleImage singleImg = _singleImages[i];
				putText(solText, to_string(solution[i]), Point(singleImg._Pos.x + singleImg._Pos.width / 3,
															   singleImg._Pos.y + (singleImg._Pos.height / 3) * 2),
						FONT_HERSHEY_SIMPLEX, 2, Scalar(255, 255, 255), 3);
			}
		}

		// change input and output matrix
		const Mat transformMatrix = getPerspectiveTransform(_outputQuad, _inputQuad);
		// Apply the Perspective Transform just found to the src image
		warpPerspective(solText, warped, transformMatrix, warped.size(), CV_INTER_AREA);

		Mat sourceWithSol = _sourceImage.clone();

		// combine solution image with source image
		addWeighted(sourceWithSol, 0.5, warped, 0.5, 0.0, sourceWithSol);

		imshow("Solution", sourceWithSol);

		waitKeyEx(0);

		destroyAllWindows();
	}
}
