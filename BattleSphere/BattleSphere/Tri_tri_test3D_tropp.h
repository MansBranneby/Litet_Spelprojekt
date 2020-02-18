#pragma once
inline void vector4Transform(float* dest, float* v1, DirectX::XMMATRIX mat4x4)
{
	dest[0] = v1[0] * mat4x4.r[0].m128_f32[0] + v1[1] * mat4x4.r[1].m128_f32[0] + v1[2] * mat4x4.r[2].m128_f32[0] + v1[3] * mat4x4.r[3].m128_f32[0];
	dest[1] = v1[0] * mat4x4.r[0].m128_f32[1] + v1[1] * mat4x4.r[1].m128_f32[1] + v1[2] * mat4x4.r[2].m128_f32[1] + v1[3] * mat4x4.r[3].m128_f32[1];
	dest[2] = v1[0] * mat4x4.r[0].m128_f32[2] + v1[1] * mat4x4.r[1].m128_f32[2] + v1[2] * mat4x4.r[2].m128_f32[2] + v1[3] * mat4x4.r[3].m128_f32[2];
	dest[3] = v1[0] * mat4x4.r[0].m128_f32[3] + v1[1] * mat4x4.r[1].m128_f32[3] + v1[2] * mat4x4.r[2].m128_f32[3] + v1[3] * mat4x4.r[3].m128_f32[3];
}

inline void subtractVector(float* dest, float* v1, float* v2)
{
	dest[0] = v1[0] - v2[0];
	dest[1] = v1[1] - v2[1];
	dest[2] = v1[2] - v2[2];
}

inline void addVector(float* dest, float* v1, float* v2)
{
	dest[0] = v1[0] + v2[0];
	dest[1] = v1[1] + v2[1];
	dest[2] = v1[2] + v2[2];
}

inline void scaleAddVector(float* dest, float s1, float* v1, float s2, float* v2)
{
	dest[0] = s1 * v1[0] + s2 * v2[0];
	dest[1] = s1 * v1[1] + s2 * v2[1];
}

inline void crossVector(float* dest, float* v1, float* v2)
{
	dest[0] = v1[1] * v2[2] - v1[2] * v2[1];
	dest[1] = v1[2] * v2[0] - v1[0] * v2[2];
	dest[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

inline bool edge_edge_test(float* V1, float* U1, float* U2, float Ax, float Ay, int index0, int index1)
{
	float  Bx, By, Cx, Cy, e, d, f;

	Bx = U1[index0] - U2[index0];
	By = U1[index1] - U2[index1];
	Cx = V1[index0] - U1[index0];
	Cy = V1[index1] - U1[index1];
	f = Ay * Bx - Ax * By;
	d = By * Cx - Bx * Cy;
	if ((f > 0.0f && d >= 0.0f && d <= f) || (f < 0.0f && d <= 0.0 && d >= f))
	{
		e = Ax * Cy - Ay * Cx;
		if (f > 0.0f)\
		{
			if (e >= 0.0f && e <= f) return true;
		}
		else
		{
			if (e <= 0.0f && e >= f) return true;
		}
	}

	return false;
}

inline bool edge_triangle_test(float* V1, float* V2, float* U1, float* U2, float* U3, int index0, int index1)
{
	float Ax, Ay;
	Ax = V2[index0] - V1[index0];
	Ay = V2[index1] - V1[index1];
	/* test edge U1,U2 against V1,V2 */
	if (edge_edge_test(V1, U1, U2, Ax, Ay, index0, index1)) return true;
	/* test edge U1,U2 against V1,V2 */
	else if (edge_edge_test(V1, U2, U3, Ax, Ay, index0, index1)) return true;
	/* test edge U2,U1 against V1,V2 */
	else if (edge_edge_test(V1, U3, U1, Ax, Ay, index0, index1)) return true;

	return false;
}

inline bool point_triangle_test(float* V1, float* U1, float* U2, float* U3, int index0, int index1)
{
	float a, b, c, d1, d2, d3;
	/* is T1 completly inside T2? */
	/* check if V0 is inside tri(U0,U1,U2) */
	a = U2[index1] - U1[index1];
	b = -(U2[index0] - U1[index0]);
	c = -a * U1[index0] - b * U1[index1];
	d1 = a * V1[index0] + b * V1[index1] + c;

	a = U3[index1] - U2[index1];
	b = -(U3[index0] - U2[index0]);
	c = -a * U2[index0] - b * U2[index1];
	d2 = a * V1[index0] + b * V1[index1] + c;

	a = U1[index1] - U3[index1];
	b = -(U1[index0] - U3[index0]);
	c = -a * U3[index0] - b * U3[index1];
	d3 = a * V1[index0] + b * V1[index1] + c;
	if (d1 * d2 > 0.0f)
	{
		if (d1 * d3 > 0.0f) return true;
	}

	return false;
}

bool tri_tri_coplanarTest(float v1[3], float v2[3], float v3[3], float u1[3], float u2[3], float u3[3], float n[3])
{
	int index0, index1;
	float absN[3];

	absN[0] = fabs(n[0]);
	absN[1] = fabs(n[1]);
	absN[2] = fabs(n[2]);

	if (absN[0] > absN[1])
	{
		if (absN[0] > absN[2])
		{
			index0 = 1;
			index1 = 2;
		}
		else
		{
			index0 = 0;
			index1 = 1;
		}
	}
	else
	{
		if (absN[2] > absN[1])
		{
			index0 = 0;
			index1 = 1;
		}
		else
		{
			index0 = 0;
			index1 = 2;
		}
	}

	if (edge_triangle_test(v1, v2, u1, u2, u3, index0, index1)) return true;
	else if (edge_triangle_test(v2, v3, u1, u2, u3, index0, index1)) return true;
	else if (edge_triangle_test(v3, v1, u1, u2, u3, index0, index1)) return true;
	else if (point_triangle_test(v1, u1, u2, u3, index0, index1)) return true;
	else if (point_triangle_test(u1, v1, v2, v3, index0, index1)) return true;

	return false;
}

bool seg_line_test(float* q, float* r, float scalingFactor, float* p1, float* p2)
{
	float gama1, gama2, gama3;
	float alpha1, alpha2;
	bool alpha1_legal, alpha2_legal;
	float det1, det2, det3;
	float pp1[3], pp2[3];
	pp1[0] = scalingFactor * p1[0];
	pp1[1] = scalingFactor * p1[1];
	pp2[0] = scalingFactor * p2[0];
	pp2[1] = scalingFactor * p2[1];
	det1 = pp1[0] * q[1] - q[0] * pp1[1];
	gama1 = (pp1[0] * r[1] - r[0] * pp1[1]) * det1;
	alpha1 = (r[0] * q[1] - q[0] * r[1]) * det1;
	alpha1_legal = (alpha1 >= 0) && (alpha1 <= (det1 * det1) && (det1 != 0));
	det2 = pp2[0] * q[1] - q[0] * pp2[1];
	alpha2 = (r[0] * q[1] - q[0] * r[1]) * det2;
	gama2 = (pp2[0] * r[1] - r[0] * pp2[1]) * det2;
	alpha2_legal = (alpha2 >= 0) && (alpha2 <= (det2 * det2) && (det2 != 0));
	det3 = det2 - det1;
	gama3 = ((pp2[0] - pp1[0]) * (r[1] - pp1[1]) - (r[0] - pp1[0]) * (pp2[1] - pp1[1])) * det3;
	if (alpha1_legal)
	{
		if (alpha2_legal)
		{
			if (((gama1 <= 0) && (gama1 >= -(det1 * det1))) || ((gama2 <= 0) && (gama2 >= -(det2 * det2))) || (gama1 * gama2 < 0)) return true;
		}
		else
		{
			if (((gama1 <= 0) && (gama1 >= -(det1 * det1))) || ((gama3 <= 0) && (gama3 >= -(det3 * det3))) || (gama1 * gama3 < 0)) return true;
		}
	}
	else
		if (alpha2_legal)
		{
			if (((gama2 <= 0) && (gama2 >= -(det2 * det2))) || ((gama3 <= 0) && (gama3 >= -(det3 * det3))) || (gama2 * gama3 < 0)) return true;
		}

	return false;
}

// P: position of a vertex in triangle A
// P: position of a vertex in triangle B
// p1, p2, q1, q2: vector between the vertex positions (eg. p1 is a vector between P and another vertex in the same triangle)
bool tri_tri_intersect3D(float P1[3], float p1[3], float p2[3], float Q1[3], float q1[3], float q2[3])
{
	// Solve equation for all of B's edges: P + alpha0 * p1 + alpha1 * p2 = Q_i + beta_i * q_i, { i | 1 <= i <= 3 } 
	// alpha0, alpha1 & beta_i: scalars
	float minor1, minor2, minor3;
	float detQ1, detQ2, detQ3, detR1, detR3;
	// r = Q_i - P
	float r1[3], r4[3];
	float beta1, beta2, beta3;
	bool beta1Legal, beta2Legal;
	float scalingFactor;
	// T: intersections point between an edge in triangle A and plane B
	// t: connects T to the seconds point of intersection
	float t[3];
	subtractVector(r1, Q1, P1);

	// STEP 1.
	// Calculate three determinants
	minor1 = p1[1] * p2[2] - p2[1] * p1[2];
	minor2 = p1[0] * p2[2] - p2[0] * p1[2];
	minor3 = p1[0] * p2[1] - p2[0] * p1[1];
	detQ1 = q1[0] * minor1 - q1[1] * minor2 + q1[2] * minor3;
	detQ2 = q2[0] * minor1 - q2[1] * minor2 + q2[2] * minor3;
	detR1 = -r1[0] * minor1 + r1[1] * minor2 - r1[2] * minor3;

	// If all detQi (1 <= i <= 3) zero then triangle A and B are coplanar
	if (detQ1 == 0.0f && detQ2 == 0.0f)
	{
		if (detR1 != 0.0f)
			return false;

		float P2[3], P3[3], Q2[3], Q3[3], n[3];

		// Calculate vertices of triangle A and B
		addVector(P2, P1, p1);
		addVector(P3, P1, p2);
		addVector(Q2, Q1, q1);
		addVector(Q3, Q1, q2);
		crossVector(n, p1, p2);

		return tri_tri_coplanarTest(P1, P2, P3, Q1, Q2, Q3, n);
	}

	// STEP 2.
	// Reject further tests if betas are illegal
	beta1 = detQ2 * detR1;
	beta2 = detQ1 * detR1;
	beta1Legal = beta2 >= 0.0f && beta2 <= detQ1 * detQ1 && detQ1 != 0.0f;
	beta2Legal = beta1 >= 0.0f && beta1 <= detQ2 * detQ2 && detQ2 != 0.0f;

	if (beta1Legal == false && beta2Legal == false)
		return false;

	detQ3 = detQ2 - detQ1;
	detR3 = detR1 - detQ1;

	//STEP 3.
	// Construct line segment beteween triangle A and B's plane
	if (beta1Legal == true && beta2Legal == true)
	{
		scalingFactor = detQ1 * detQ2;
		scaleAddVector(t, beta2, q2, (-beta1), q1);
	}
	else if (beta1Legal == true && beta2Legal == false)
	{
		scalingFactor = detQ1 * detQ3;
		beta1 = beta1 - beta2;
		beta3 = detR3 * detQ1;
		scaleAddVector(t, (scalingFactor - beta3 - beta1), q1, beta3, q2);
	}
	else if (beta1Legal == false && beta2Legal == true)
	{
		scalingFactor = detQ2 * detQ3;
		beta2 = beta1 - beta2;
		beta3 = detR3 * detQ2;
		scaleAddVector(t, (scalingFactor - beta3), q1, (beta3 - beta2), q2);
		q1 = q2;
		beta1 = beta2;
	}
	scaleAddVector(r4, scalingFactor, r1, beta1, q1);
	if (seg_line_test(t, r4, scalingFactor, p1, p2)) return true;

	return false;
}