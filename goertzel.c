#include <Python.h>
#include <stdio.h>
#include "numpy/arrayobject.h"

#if PY_MAJOR_VERSION >= 3
#define PY3K
#endif

// numpy handle funcions
int  not_doublevector(PyArrayObject *vec)  {
    if (vec->descr->type_num != NPY_DOUBLE || vec->nd != 1)  {
        PyErr_SetString(PyExc_ValueError,
            "In not_doublevector: array must be of type Float and 1 dimensional (n).");
        return 1;  }
    return 0;
};
double *pyvector_to_Carrayptrs(PyArrayObject *arrayin)  {
  return (double *) arrayin->data;  /* pointer to arrayin data as double */
};

// goertzel implementation
void goertzelFilter_c(double *sample_data, float target_freq,int sampling_rate, int numSamples, float* mag, float* ang)
{
    int     k,i;
    float   floatnumSamples;
    float   omega,sine,cosine,coeff,q0,q1,q2,real,imag;

    float scalingFactor = numSamples / 2.0;

    floatnumSamples = (float) numSamples;
    k = (int) (0.5 + ((floatnumSamples * target_freq) / (float)sampling_rate));
    omega = (2.0 * M_PI * k) / floatnumSamples;
    sine = sin(omega);
    cosine = cos(omega);
    coeff = 2.0 * cosine;
    q0=0;
    q1=0;
    q2=0;

    for(i=0; i<numSamples; i++)
    {
        q0 = coeff * q1 - q2 + sample_data[i];
        q2 = q1;
        q1 = q0;
    }

    // calculate the real and imaginary results
    real = (q1 * cosine - q2) / scalingFactor;
    imag = (q1 * sine) / scalingFactor;
    *ang = atan2(real,imag);
    *mag = sqrtf(real*real + imag*imag);

};


// module functions
static PyObject* goertzelFilter(PyObject *self, PyObject *args)
{
    PyArrayObject *data_np = NULL;
    float freq;
    int samplerate, n;
    int numSamples = 0;
    double *sample;
    float mag, ang;

    if (!PyArg_ParseTuple(args,"O!fi|i",&PyArray_Type,&data_np,&freq,&samplerate,&numSamples)) 
        return NULL;
    if (NULL == data_np)  return NULL;
    if (not_doublevector(data_np)) return NULL;
    
    sample = pyvector_to_Carrayptrs(data_np);
    n=data_np->dimensions[0]; //get vector dimentions
    if(numSamples>0){
        n=numSamples;
    }

    goertzelFilter_c(sample,freq,samplerate,n,&mag,&ang);
    return Py_BuildValue("[ff]",mag,ang);

}

// registration table
static PyMethodDef goertzel_methods[]={
    {"goertzelFilter", (PyCFunction) goertzelFilter, METH_VARARGS, "calculates the goertzel Filter for a given numpy array, frequency and sample rate, Returns the (magnitude, phase)"},
    {NULL, NULL, 0, NULL}
};


#ifdef PY3K
// module definition structure for python3
static struct PyModuleDef goertzelmodule = {
    PyModuleDef_HEAD_INIT,
    "goertzel",
    "mod doc",
    -1,
    goertzel_methods
};
// module initializer for python3
PyMODINIT_FUNC PyInit_goertzel(void)
{
    import_array();
    return PyModule_Create(&goertzelmodule);
}
#else
// module initializer for python2
PyMODINIT_FUNC initgoertzel() {
    Py_InitModule3("goertzel", goertzel_methods, "mod doc");
}
#endif