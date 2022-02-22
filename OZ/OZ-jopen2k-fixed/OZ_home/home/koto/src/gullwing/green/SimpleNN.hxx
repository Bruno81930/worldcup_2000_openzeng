// SimpleNN.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(SIMPLENN_HXX__INCLUDED) // {
#define SIMPLENN_HXX__INCLUDED

/////////////////////////////////////////////////////////////////////////////
// SimpleNN

template <int size, int size2>
class SimpleNN
{
private:
	SimpleNN(const SimpleNN<size, size2>&);
	SimpleNN<size, size2>& operator= (const SimpleNN<size, size2>&);
	bool operator== (const SimpleNN<size, size2>&) const;
	
protected:
	double m_input[size+1];
	double m_hidden[size2+1];
	double m_output[1];
	double m_i2h[size2][size+1];
	double m_h2o[1][size2+1];
	static double sigmoid(double d) {
		return 1.0 / (1.0 + exp(-d));
	}
	
public:
	void input(int index, double value) {
		ASSERT(0 <= index && index < size);
		m_input[index] = value;
	}

	virtual ~SimpleNN() {
	}
	SimpleNN() {
		int i, j;
		for(i=0; i<=size; i++)
			m_input[i] = 1;
		for(i=0; i<=size2; i++)
			m_hidden[i] = 1;
		m_output[0] = 1;
		for(i=0; i<size2; i++) {
			for(j=0; j<=size; j++)
				m_i2h[i][j] = (rand() % 201) * 0.01 - 1.0;
		}
		for(i=0; i<1; i++) {
			for(j=0; j<=size2; j++)
				m_h2o[i][j] = (rand() % 201) * 0.01 - 1.0;
		}
	}
	double perform() {
		int i, j;
		for(i=0; i<size2; i++) {
			double sum = 0.0;
			for(j=0; j<=size; j++)
				sum += m_input[j] * m_i2h[i][j];
			m_hidden[i] = sigmoid(sum);
		}
		
		for(i=0; i<1; i++) {
			double sum = 0.0;
			for(j=0; j<=size2; j++)
				sum += m_hidden[j] * m_h2o[i][j];
			m_output[i] = sigmoid(sum);
		}
		
		return m_output[0];
	}
	void backPropagation(double alpha, double teacher) {
		int i, j;
		double e = teacher - m_output[0];
		double delta = e * m_output[0] * (1 - m_output[0]);
		for(i=0; i<size2+1; i++)
			m_h2o[0][i] += alpha * m_hidden[i] * delta;

		for(j=0; j<size2; j++) {
			double d = m_hidden[j] * (1 - m_hidden[j]) * m_h2o[0][j] * delta;
			for(i=0; i<size+1; i++) {
				m_i2h[j][i] += alpha * m_input[i] * d;
			}
		}
	}
};

/////////////////////////////////////////////////////////////////////////////

#endif // } !defined(SIMPLENN_HXX__INCLUDED)
