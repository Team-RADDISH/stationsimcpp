//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_POINT2D_HPP
#define STATIONSIM_POINT2D_HPP

#include "stationsim_export.h"

namespace station_sim {

	class STATIONSIM_EXPORT Point2D {
	public:
		float x, y;

		Point2D();

		Point2D(float x, float y);
	};

}

#endif //STATIONSIM_POINT2D_HPP
