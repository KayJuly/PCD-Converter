#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream> 

using namespace std;

struct pt_t {
	float x, y, z, r, g, b;
	pt_t() { ; }
	pt_t(float _x, float _y, float _z, float _r, float _g, float _b) {
		x = _x;
		y = _y;
		z = _z;
		r = _r;
		g = _g;
		b = _b;
	}
};
struct pt_o {
	float x, y, z;
	int rgb;
	pt_o() { ; }
	pt_o(float _x, float _y, float _z, int _rgb) {
		x = _x;
		y = _y;
		z = _z;
	    rgb = _rgb;
	}
};

#define RGB(r, g, b) ((int(r) << 16) + (int(g) << 8) + int(b) )

int main(int argc, const char * argv[]) {

	vector<pt_t> pts;
	pt_t pt(0, 0, 0, 0, 0, 0);
	pt_o po(0, 0, 0, 0);

	int i;
	if (argc != 3) {
		cout << "Error! PCDConverter needs 3 parameters.";
		return 0;
	}

	string infile = argv[1];
	string otfile = argv[2];

	fstream ifs(infile, ios::in);
	ofstream ofs(otfile, ios::out | ios::binary);


	if (!ifs) {
		cout << "Error! In File can not be opened" << endl;
		return 0;
	}
	if (!ofs) {
		cout << "Error! Out File can not be opened" << endl;
		return 0;
	}

	while (!ifs.eof()) {
		vector<string> rec;
		string str;
		string buf;

		ifs >> buf;
		istringstream streambuffer(buf);
		while (getline(streambuffer, str, ',')) {
			rec.push_back(str);
		}
		if (rec.size() != 0 ) {
			pt = pt_t(
				stof(rec[0]), stof(rec[1]), stof(rec[2]),
				stof(rec[3]), stof(rec[4]), stof(rec[5])
			);
			pts.push_back(pt);
		}
	}

	//------- binary
	ofs << "# .PCD v.7 - Point Cloud Data file format" << std::endl;
	ofs << "VERSION .7\nFIELDS x y z rgb\nSIZE 4 4 4 4\nTYPE F F F U\nCOUNT 1 1 1 1" << std::endl;
	ofs << "WIDTH " << pts.size() << std::endl;
	ofs << "HEIGHT 1\nVIEWPOINT 0 0 0 1 0 0 0" << std::endl;
	ofs << "POINTS " << pts.size() << std::endl;
	ofs << "DATA binary" << std::endl;

	for (int i = 0; i < pts.size(); i++) {

		unsigned int col = ((unsigned int)pts[i].b << 16 | (unsigned int)pts[i].g << 8 | (unsigned int)pts[i].r);
		po.x = pts[i].x;
		po.y = pts[i].y;
		po.z = pts[i].z;
		po.rgb = col;
		ofs.write((char*)&po, sizeof(po));
	}

/*	
	//------- ascii
	ofs << "# .PCD v.7 - Point Cloud Data file format" << std::endl;
	ofs << "VERSION .7\nFIELDS x y z rgb\nSIZE 4 4 4 4\nTYPE F F F U\nCOUNT 1 1 1 1" << std::endl;
	ofs << "WIDTH " << pts.size() << std::endl;
	ofs << "HEIGHT 1\nVIEWPOINT 0 0 0 1 0 0 0" << std::endl;
	ofs << "POINTS " << pts.size() << std::endl;
	ofs << "DATA ascii" << std::endl;

	// data
	for (int i = 0; i < pts.size(); i++) {
		unsigned int col = ((unsigned int)pts[i].b << 16 | (unsigned int)pts[i].g << 8 | (unsigned int)pts[i].r);
		ofs << pts[i].x << " " << pts[i].y << " " << pts[i].z << " " << col << endl;
	}
*/
	ofs.close();

	return 0;
}

