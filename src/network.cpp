#include "network.h"
#include "random.h"
#include <cmath>
using namespace std; 

void Network::resize(const size_t &n)
{
	values.clear();
	values.resize(n);
	RNG.normal(values);
}

bool Network::add_link(const size_t& a, const size_t& b)
{
	bool nodes_existe(true);
	bool pas_de_lien(true);
	if((a >= size()) or (b >= size()))
	{
		nodes_existe = false;
	}
	if(a == b)
	{
		pas_de_lien = false;
	}
	auto range = links.equal_range(a);
	for(auto i = range.first; i != range.second; ++i)
	{
		if((i->second) == b)
		{
		pas_de_lien = false;
		}
	}
	if(nodes_existe and pas_de_lien)
	{
		links.insert({a, b});
		return true;
	} 
	else
	{
		return false;
	}	
}

size_t Network::random_connect(const double& mean_deg)
{
	bool pas_de_lien(true);
	size_t total_links(0);
	for(size_t i(0); i < size(); ++i)
	{
		links.erase(i);
	}	
	for(size_t i(0); i < size(); ++i)
	{
		int nb_link(RNG.poisson(mean_deg));
		if(nb_link > size())
		{
			nb_link = size();
		}
		total_links += nb_link;

		for(int j(0); j < nb_link; ++j)
		{
			add_link(i, floor(RNG.uniform_double(0, size())));
		}
	}
	return total_links;
}

size_t Network::set_values(const std::vector<double>& vec)
{
	if(vec.size() <= size())
	{
		for(size_t i(0); i < vec.size(); ++i)
		{
			values[i] = vec[i];
		}

		return vec.size();
	}
	if(vec.size() > size())
	{
		for(size_t i(0); i < size(); ++i)
		{
			values[i] = vec[i];
		}
		return size();
	}
}

size_t Network::size() const
{
	return values.size();
}

size_t Network::degree(const size_t &_n) const
{
	return links.count(_n)*2;
}

double Network::value(const size_t &_n) const
{
	if(_n < size())
	{
		return values[_n];
	}
	else
	{
		throw "ce nombre n'existe pas !";
	}
}

vector<double> Network::sorted_values() const
{
	vector<double> ordo;
	ordo = values;
	
	double valeurtemporaire;
	for(unsigned int i(0); i < ordo.size(); ++i)
	{
		for(unsigned int j(i); j < ordo.size(); ++j)
		{
			if(ordo[j] > ordo[i])
			{
				valeurtemporaire = ordo[i];
				ordo[i] = ordo[j];
                ordo[j] = valeurtemporaire;
			}
		}
	}
	return ordo;
}

vector<size_t> Network::neighbors(const size_t& node) const
{
	vector<size_t> neighb;
	auto range = links.equal_range(node);
	for(auto i = range.first; i != range.second; ++i)
	{
		neighb.push_back(i->second);
	}	
	for(size_t j(0); j < size(); ++j)
	{
		auto range = links.equal_range(j);
		for(auto k = range.first; k != range.second; ++k)
		{
			if(k->second == node)
			{
				neighb.push_back(k->first);
			}
		}	
	}
	return neighb;
}
