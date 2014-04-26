#include "EU4World.h"
#include <fstream>
#include "../Log.h"
#include "../Configuration.h"
#include "../Mapper.h"
#include "../Parsers/Object.h"
#include "EU4Province.h"
#include "EU4Country.h"
#include "EU4Diplomacy.h"
#include "EU4Version.h"

EU4World::EU4World(Object* obj)
{
	vector<Object*> versionObj = obj->getValue("savegame_version");
	if (versionObj.size() > 0)
	{
		version = new EU4Version(versionObj[0]);
	}
	else
	{
		version = new EU4Version();
	}

	string keyProv;
	string keyCoun;

	// Get Provinces and then get Countries
	provinces.clear();
	countries.clear();
	
	vector<Object*> provincesObj = obj->getValue("provinces");				// Set object to be base leaf named provinces
	if (provincesObj.size() > 0)											// If object leaf exists, proceed
	{
		vector<Object*> provincesLeaves = provincesObj[0]->getLeaves();		// Set object to be leaves of listed object
		for (unsigned int j = 0; j < provincesLeaves.size(); j++)			// For entire set of leaves, preform following
		{
			keyProv = (provincesLeaves[j])->getKey();						// Get key for current province.  In this case a negative number.
		
			if (atoi(keyProv.c_str()) < 0)									// Check if key is a negative value (EU4 style)
			{
				EU4Province* province = new EU4Province((provincesLeaves[j]));		// Set values of leaf to the class province
				provinces.insert(make_pair(province->getNum(), province));			// insert num indentifier (made to be positive in EU4Province now)
			}
		}
	}

	vector<Object*> countriesObj = obj->getValue("countries");
	if (countriesObj.size() > 0)											// If object leaf exists, proceed
	{
		vector<Object*> countriesLeaves = countriesObj[0]->getLeaves();
		for (unsigned int j = 0; j < countriesLeaves.size(); j++)
		{
			keyCoun = countriesLeaves[j]->getKey();

			if ((keyCoun == "---") || (keyCoun == "REB") || (keyCoun == "PIR") || (keyCoun == "NAT"))
			{
				continue;
			}
			else
			{
				EU4Country* country = new EU4Country(countriesLeaves[j]);
				countries.insert(make_pair(country->getTag(), country));
			}
		}
	}
	
/*	vector<Object*> provinceObj = obj->getValue("provinces");
	if (provinceObj.size() > 0)
	{
		vector<Object*> lowerProvinces = provinceObj[0]->getLeaves();
		for (vector<Object*>::iterator i = lowerProvinces.begin(); i != lowerProvinces.end(); i++)
		{
			keyProv = (*i)->getKey();
			
			// Is this a negative value? If so, must be a province
			if (atoi(keyProv.c_str()) < 0)
			{
				EU4Province* province = new EU4Province((*i));
				provinces.insert(make_pair(province->getNum(), province));
			}
		}
	}
	*/

	/*	vector<Object*> provinceObj = obj->getValue("provinces");
	for (unsigned int i = 0; i < provinceObj.size(); i++) // loop through all the sections marked provinces
	if (provinceObj.size() > 0)
	{
		vector<Object*> provinceObjlist = provinceObj[0]->getLeaves();
		for (unsigned int j = 0; j < provinceObjlist.size(); j++)
		{
			keyProv = (provinceObjlist[j])->getKey();
			
			// Is this a negative value? If so, must be a province
			if (atoi(keyProv.c_str()) < 0)
			{
				EU4Province* province = new EU4Province((provinceObjlist[j]));
				provinces.insert(make_pair(province->getNum(), province));
			}
		}
	}

	vector<Object*> countriesObj = obj->getValue("countries");
	if (countriesObj.size() > 0)
	{
		vector<Object*> lowerCountries = countriesObj[0]->getLeaves();
		for (vector<Object*>::iterator i = lowerCountries.begin(); i != lowerCountries.end(); i++)
		{
			keyCoun = (*i)->getKey();

			// Countries are three uppercase characters
			if ((keyCoun.size() == 3) && 
					(keyCoun.c_str()[0] >= 'A') && (keyCoun.c_str()[0] <= 'Z') && 
					(keyCoun.c_str()[1] >= 'A') && (keyCoun.c_str()[1] <= 'Z') && 
					(keyCoun.c_str()[2] >= 'A') && (keyCoun.c_str()[2] <= 'Z')
				  )
			{
				EU4Country* country = new EU4Country((*i));
				countries.insert(make_pair(country->getTag(), country));
			}
		}
	}
	*/

/*	// Old Code
	string key;
	vector<Object*> leaves = obj->getLeaves();

	provinces.clear();
	countries.clear();
	for (unsigned int i = 0; i < leaves.size(); i++)
	{
		key = leaves[i]->getKey();

		// Is this a numeric value? If so, must be a province
		if (atoi(key.c_str()) > 0)
		{
			EU3Province* province = new EU3Province(leaves[i]);
			provinces.insert(make_pair(province->getNum(), province));
		}

		// Countries are three uppercase characters
		else if ((key.size() == 3) && 
					(key.c_str()[0] >= 'A') && (key.c_str()[0] <= 'Z') && 
					(key.c_str()[1] >= 'A') && (key.c_str()[1] <= 'Z') && 
					(key.c_str()[2] >= 'A') && (key.c_str()[2] <= 'Z')
				  )
		{
			EU3Country* country = new EU3Country(leaves[i]);
			countries.insert(make_pair(country->getTag(), country));
		}
	}
*/

	// add province owner info to countries
	for (map<int, EU4Province*>::iterator i = provinces.begin(); i != provinces.end(); i++)
	{
		map<string, EU4Country*>::iterator j = countries.find( i->second->getOwnerString() );
		if (j != countries.end())
		{
			j->second->addProvince(i->second);
			i->second->setOwner(j->second);
		}
	}

	// add province core info to countries
	for (map<int, EU4Province*>::iterator i = provinces.begin(); i != provinces.end(); i++)
	{
		vector<EU4Country*> cores = i->second->getCores(countries);
		for (vector<EU4Country*>::iterator j = cores.begin(); j != cores.end(); j++)
		{
			(*j)->addCore(i->second);
		}
	}

	vector<Object*> diploObj = obj->getValue("diplomacy");
	if (diploObj.size() > 0)
	{
		diplomacy = new EU4Diplomacy(diploObj[0]);
	}
	else
	{
		diplomacy = new EU4Diplomacy;
	}
}

EU4Country* EU4World::getCountry(string tag) const
{
	map<string, EU4Country*>::const_iterator i = countries.find(tag);
	if (i != countries.end())
	{
		return i->second;
	}
	else
	{
		return NULL;
	}
}


EU4Province* EU4World::getProvince(int provNum) const
{
	map<int, EU4Province*>::const_iterator i = provinces.find(provNum);
	if (i != provinces.end())
	{
		return i->second;
	}
	else
	{
		return NULL;
	}
}


void EU4World::removeCountry(string tag)
{
	countries.erase(tag);
}


void EU4World::resolveRegimentTypes(const RegimentTypeMap& rtMap)
{
	for (map<string, EU4Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
	{
		itr->second->resolveRegimentTypes(rtMap);
	}
}


void EU4World::checkAllProvincesMapped(const inverseProvinceMapping& inverseProvinceMap) const
{
	for (map<int, EU4Province*>::const_iterator i = provinces.begin(); i != provinces.end(); i++)
	{
		inverseProvinceMapping::const_iterator j = inverseProvinceMap.find(i->first);
		if (j == inverseProvinceMap.end())
		{
			log("	Error: no mapping for province #%d\n", i->first);
		}
	}
}