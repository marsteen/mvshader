

template <typename Tfloat>
static void GetCirclePoints(Tfloat AlphaStart, Tfloat AlphaEnd, Tfloat radius, int Segments, Tfloat offx, Tfloat offy,  std::vector<CVector2<Tfloat> >& vec)
{
	Tfloat Alpha     = DEG_TO_RAD(AlphaStart);
	Tfloat AlphaPlus = DEG_TO_RAD(AlphaEnd - AlphaStart) / Segments;
	
	for (int n = 0; n <= Segments; n++)
	{
		CVector2<Tfloat> CirclePoint;
    CirclePoint.x = cos(Alpha) * radius + offx;
    CirclePoint.y = sin(Alpha) * radius + offy;
    Alpha += AlphaPlus;		
		vec.push_back(CirclePoint);		
	}
}