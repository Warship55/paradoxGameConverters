/*Copyright (c) 2016 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/



#ifndef HOI4STATE_H_
#define HOI4STATE_H_



#include <vector>
#include <string>
using namespace std;



class HoI4State
{
	public:
		HoI4State(int _ID, string _ownerTag, float _Manpower, double _CivFactories, int _MilFactories, string _catagory, int _raillevel, int _navalbase, int _navallocation);

		void output(string filename);
		void		setResources(string _resources) { resources = _resources; }
		void			addProvince(int province)	{ provinces.push_back(province); }
		vector<int> getProvinces() const			{ return provinces; }
		string		getOwner() const				{ return ownerTag; }
		float		getManpower() const				{ return manpower; }
		int			getID()							{ return ID; }
		int			getNavalLocation() const		{ return navallocation; }

	private:
		int			ID;
		vector<int>	provinces;
		string		ownerTag;
		float			manpower;
		double		civFactories;
		int			milFactories;
		string catagory;
		int raillevel;
		string resources;
		int navalbase;
		int navallocation;

};



#endif // HOI4STATE_H_