/********************************************
Autor: Thorsten Liebig
Version: 1.1
Stand: 30.5.05
*********************************************/

#include "VTKPrimitives.h"
#include "vtkRenderer.h"
#include "vtkActorCollection.h"

#include "vtkPolyDataMapper.h"
#include "vtkDoubleArray.h"
#include "vtkLODActor.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkCubeSource.h"
#include "vtkCylinderSource.h"
#include "vtkSphereSource.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkRotationalExtrusionFilter.h"
#include "vtkLinearExtrusionFilter.h"
#include "vtkArrowSource.h"
#include "vtkSTLReader.h"
#include "vtkVectorText.h"
#include "vtkFollower.h"
#include "vtkTriangleFilter.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkCollectionIterator.h"
#include "vtkConeSource.h"


VTKPrimitives::VTKPrimitives(vtkRenderer *Renderer)
{
	ren = Renderer;

	ActorColl = vtkActorCollection::New();
}

VTKPrimitives::~VTKPrimitives()
{
	ActorColl->InitTraversal();
	vtkActor* act=NULL;
	while (act=ActorColl->GetNextActor())
	{
		ren->RemoveActor(act);
		act->Delete();
	}
	ActorColl->Delete();
	ActorColl=NULL;
}

VTKPrimitives::VTKPrimitives()
{
}


void VTKPrimitives::AddCube(double *dCoords, double *dRGB, double dOpacity)
{ //complete
	vtkCubeSource *Source = vtkCubeSource::New();
	vtkPolyDataMapper *SourceMapper = vtkPolyDataMapper::New();
	vtkActor *SourceActor = vtkActor::New();
	Source->SetBounds(dCoords);
	SourceMapper->SetInput(Source->GetOutput());
	SourceActor->SetMapper(SourceMapper);
	SourceActor->GetProperty()->SetColor(dRGB);
	SourceActor->GetProperty()->SetOpacity(dOpacity);
	ren->AddActor(SourceActor);
	ActorColl->AddItem(SourceActor);
	Source->Delete();
	SourceMapper->Delete();
	//SourceActor->Delete();
}

void VTKPrimitives::AddDisc(double *dCoords, unsigned int uiQtyCoords, double *dRGB, double dOpacity)
{
	unsigned int i=0,j=0,h=0;//,k=0;
//	vtkIdType pts[6][4]={{0,1,2,3}, {4,5,6,7}, {0,1,5,4},
//                        {1,2,6,5}, {2,3,7,6}, {3,0,4,7}};
	vtkPoints *points = vtkPoints::New();
	vtkCellArray *poly = vtkCellArray::New();
	vtkPolyData *profile = vtkPolyData::New();
	vtkPolyDataMapper *Mapper = vtkPolyDataMapper::New();
	vtkActor *Actor = vtkActor::New();
	//vtkFloatArray *scalars = vtkFloatArray::New();
	for (i=0; i<uiQtyCoords;i=i+2) 
	{
		points->InsertPoint(j++,dCoords[i],dCoords[uiQtyCoords+i],dCoords[2*uiQtyCoords+i]);//0
		points->InsertPoint(j++,dCoords[i+1],dCoords[uiQtyCoords+i],dCoords[2*uiQtyCoords+i]);//1
		points->InsertPoint(j++,dCoords[i+1],dCoords[uiQtyCoords+i+1],dCoords[2*uiQtyCoords+i]);//2
		points->InsertPoint(j++,dCoords[i],dCoords[uiQtyCoords+i+1],dCoords[2*uiQtyCoords+i]);//3
		points->InsertPoint(j++,dCoords[i],dCoords[uiQtyCoords+i],dCoords[2*uiQtyCoords+i+1]);//4
		points->InsertPoint(j++,dCoords[i+1],dCoords[uiQtyCoords+i],dCoords[2*uiQtyCoords+i+1]);//5
		points->InsertPoint(j++,dCoords[i+1],dCoords[uiQtyCoords+i+1],dCoords[2*uiQtyCoords+i+1]);//6
		points->InsertPoint(j++,dCoords[i],dCoords[uiQtyCoords+i+1],dCoords[2*uiQtyCoords+i+1]);//7
		poly->InsertNextCell(4);poly->InsertCellPoint(0+i*4);poly->InsertCellPoint(1+i*4);poly->InsertCellPoint(2+i*4);poly->InsertCellPoint(3+i*4);
		poly->InsertNextCell(4);poly->InsertCellPoint(4+i*4);poly->InsertCellPoint(5+i*4);poly->InsertCellPoint(6+i*4);poly->InsertCellPoint(7+i*4);
		poly->InsertNextCell(4);poly->InsertCellPoint(0+i*4);poly->InsertCellPoint(1+i*4);poly->InsertCellPoint(5+i*4);poly->InsertCellPoint(4+i*4);
		poly->InsertNextCell(4);poly->InsertCellPoint(1+i*4);poly->InsertCellPoint(2+i*4);poly->InsertCellPoint(6+i*4);poly->InsertCellPoint(5+i*4);
		poly->InsertNextCell(4);poly->InsertCellPoint(2+i*4);poly->InsertCellPoint(3+i*4);poly->InsertCellPoint(7+i*4);poly->InsertCellPoint(6+i*4);
		poly->InsertNextCell(4);poly->InsertCellPoint(3+i*4);poly->InsertCellPoint(0+i*4);poly->InsertCellPoint(4+i*4);poly->InsertCellPoint(7+i*4);
		//for (h=j-8;h<j;h++) scalars->InsertTuple1(h,h%8); // not in use jet
		//fprintf(stderr,".");
	}
	profile->SetPoints(points);
	profile->SetPolys(poly);
	//profile->GetPointData()->SetScalars(scalars);
	Mapper->SetInput(profile);
	//Mapper->SetScalarRange(0,8);
	Actor->SetMapper(Mapper);
	Actor->GetProperty()->SetColor(dRGB);
	Actor->GetProperty()->SetOpacity(dOpacity);
    ren->AddActor(Actor);
	ActorColl->AddItem(Actor);

	points->Delete();
	poly->Delete();
	profile->Delete();
	Mapper->Delete();
	//Actor->Delete();
	//scalars->Delete();
}


void VTKPrimitives::AddClosedPoly(double *dCoords, unsigned int uiQtyCoords, double *dExtrusionVector, double *dRGB, double dOpacity)
{  //complete
	unsigned int i=0;
	vtkPoints *points = vtkPoints::New();
	vtkCellArray *poly = vtkCellArray::New();
	vtkPolyData *profile = vtkPolyData::New();
	vtkLinearExtrusionFilter *extrude = vtkLinearExtrusionFilter::New();
	vtkPolyDataMapper *Mapper = vtkPolyDataMapper::New();
	vtkActor *Actor = vtkActor::New();
	for (i=0; i<uiQtyCoords;i++) points->InsertPoint(i,dCoords[i],dCoords[uiQtyCoords+i],dCoords[2*uiQtyCoords+i]);
	poly->InsertNextCell(uiQtyCoords+1);
	for (i=0; i<uiQtyCoords ;i++) poly->InsertCellPoint(i);
	poly->InsertCellPoint(0);
	profile->SetPoints(points);
	profile->SetPolys(poly);
	vtkTriangleFilter *tf = vtkTriangleFilter::New();
	tf->SetInput(profile);

	extrude->SetInput(tf->GetOutput());
	extrude->SetExtrusionTypeToVectorExtrusion();
	extrude->SetVector(dExtrusionVector);
	extrude->CappingOn();

//	fprintf(stderr,"\n Vector: %f %f %f", dExtrusionVector[0],dExtrusionVector[1],dExtrusionVector[2]);

	Mapper->SetInput(extrude->GetOutput());
	Actor->SetMapper(Mapper);
	Actor->GetProperty()->SetColor(dRGB);
	Actor->GetProperty()->SetOpacity(dOpacity);
    ren->AddActor(Actor);
	ActorColl->AddItem(Actor);

	points->Delete();
	poly->Delete();
	profile->Delete();
	tf->Delete();
	extrude->Delete();

	Mapper->Delete();
	//Actor->Delete();
}

void VTKPrimitives::AddLinePoly(double *dCoords, unsigned int uiQtyCoords, unsigned int LineWidth, double *dRGB, double dOpacity)
{ //complete
	unsigned int i=0;
	vtkPoints *points = vtkPoints::New();
	vtkCellArray *poly = vtkCellArray::New();
	vtkPolyData *profile = vtkPolyData::New();
	vtkPolyDataMapper *Mapper = vtkPolyDataMapper::New();
	vtkActor *Actor = vtkActor::New();
	for (i=0; i<uiQtyCoords;i++) points->InsertPoint(i,dCoords[i],dCoords[uiQtyCoords+i],dCoords[2*uiQtyCoords+i]);
	for (i=1; i<uiQtyCoords;i++)
	{
		poly->InsertNextCell(2);
		poly->InsertCellPoint(i-1);
		poly->InsertCellPoint(i);
	}

	profile->SetPoints(points);
	profile->SetLines(poly);
	Mapper->SetInput(profile);
	Actor->SetMapper(Mapper);
	Actor->GetProperty()->SetColor(dRGB);
	Actor->GetProperty()->SetOpacity(dOpacity);
	Actor->GetProperty()->SetLineWidth(LineWidth);

	ActorColl->AddItem(Actor);
	ren->AddActor(Actor);

	points->Delete();
	poly->Delete();
	profile->Delete();
	Mapper->Delete();
	//Actor->Delete();

}
void VTKPrimitives::AddCylinder(double *dCenter, double *dExtrusionVector, float fRadius, double *dRGB, double dOpacity, int iResolution)
{//complete
	double alpha=0,beta=0;
	double length=sqrt( dExtrusionVector[0]*dExtrusionVector[0]+dExtrusionVector[1]*dExtrusionVector[1]+dExtrusionVector[2]*dExtrusionVector[2] ) ;
	//if (length==0) { fprintf(stderr," Error Cylinder Extrusion Vector ist Zero.. Abort..."); exit(1); } 
	vtkCylinderSource *Source = vtkCylinderSource::New();
	vtkTransform *transform = vtkTransform::New();
	vtkTransformPolyDataFilter *transformFilter = vtkTransformPolyDataFilter::New();
	vtkPolyDataMapper *SourceMapper = vtkPolyDataMapper::New();
	vtkActor *SourceActor = vtkActor::New();
	Source->SetResolution(iResolution);
	Source->SetRadius(fRadius);
	Source->SetHeight(length);

	alpha=VectorAngel(dExtrusionVector[0],sqrt(dExtrusionVector[1]*dExtrusionVector[1]+dExtrusionVector[2]*dExtrusionVector[2]),0,0,1,0);
	beta=VectorAngel(0,dExtrusionVector[1],dExtrusionVector[2],0,1,0);
	// cout << alpha << "   "  << beta << endl;  //debuging
	if (dExtrusionVector[0]>0) alpha=-alpha;
	if (dExtrusionVector[2]<0) beta=-beta;
	transform->Translate(dExtrusionVector[0]/2+dCenter[0],dExtrusionVector[1]/2+dCenter[1],dExtrusionVector[2]/2+dCenter[2]);
	transform->RotateWXYZ(beta,1,0,0);
	transform->RotateWXYZ(alpha,0,0,1);

	transformFilter->SetInput(Source->GetOutput());
	transformFilter->SetTransform(transform);

	SourceMapper->SetInput(transformFilter->GetOutput());
	SourceActor->SetMapper(SourceMapper);
	SourceActor->GetProperty()->SetColor(dRGB);
	SourceActor->GetProperty()->SetOpacity(dOpacity);

	ActorColl->AddItem(SourceActor);
	ren->AddActor(SourceActor);

	Source->Delete();
	transform->Delete();
	transformFilter->Delete();
	SourceMapper->Delete();
	//SourceActor->Delete();
}

void VTKPrimitives::AddSphere(double *dCenter, float fRadius, double *dRGB, double dOpacity, int iResolution)
{//complete
	vtkSphereSource *Source = vtkSphereSource::New();
	vtkPolyDataMapper *SourceMapper = vtkPolyDataMapper::New();
	vtkActor *SourceActor = vtkActor::New();
	Source->SetCenter(dCenter);
	Source->SetRadius(fRadius);
	Source->SetPhiResolution(iResolution);
	Source->SetThetaResolution(iResolution);
	SourceMapper->SetInput(Source->GetOutput());
	SourceActor->SetMapper(SourceMapper);
	SourceActor->GetProperty()->SetColor(dRGB);
	SourceActor->GetProperty()->SetOpacity(dOpacity);
	ren->AddActor(SourceActor);
	ActorColl->AddItem(SourceActor);
	Source->Delete();
	SourceMapper->Delete();
	//SourceActor->Delete();
}

void VTKPrimitives::AddArrow(double *dStart, double *dEnd, double *dRGB, double dOpacity, int iResolution)
{
	double alpha=0,beta=0;
	double dvector[3]={dEnd[0]-dStart[0],dEnd[1]-dStart[1],dEnd[2]-dStart[2]};
	double length=sqrt( dvector[0]*dvector[0]+dvector[1]*dvector[1]+dvector[2]*dvector[2] ) ;
	if (length==0) { fprintf(stderr," Error Arrow Length ist Zero.. Abort..."); exit(1); } 
	vtkArrowSource *Source = vtkArrowSource::New();
	vtkTransform *transform = vtkTransform::New();
	vtkTransformPolyDataFilter *transformFilter = vtkTransformPolyDataFilter::New();
	vtkPolyDataMapper *SourceMapper = vtkPolyDataMapper::New();
	vtkActor *SourceActor = vtkActor::New();
	Source->SetTipResolution(iResolution);
	Source->SetShaftResolution(iResolution);
	Source->SetTipLength(0.15);
	Source->SetTipRadius(0.03);
	Source->SetShaftRadius(0.01);

	alpha=VectorAngel(dvector[0],sqrt(dvector[1]*dvector[1]+dvector[2]*dvector[2]),0,0,1,0);
	beta=VectorAngel(0,dvector[1],dvector[2],0,1,0);
	if (dvector[0]>0) alpha=-alpha;
	if (dvector[2]<0) beta=-beta;
//	fprintf(stderr,"aplha (um z): %f; beta (um x): %f",alpha,beta);
	transform->RotateWXYZ(beta,1,0,0);
	transform->RotateWXYZ(alpha,0,0,1);
	transform->RotateWXYZ(90,0,0,1);
	
	transformFilter->SetInput(Source->GetOutput());
	transformFilter->SetTransform(transform);

	SourceMapper->SetInput(transformFilter->GetOutput());
	SourceActor->SetMapper(SourceMapper);
	SourceActor->GetProperty()->SetColor(dRGB);
	SourceActor->GetProperty()->SetOpacity(dOpacity);
	SourceActor->SetScale(length);
	SourceActor->SetPosition(dStart);
	ActorColl->AddItem(SourceActor);
	ren->AddActor(SourceActor);

	Source->Delete();
	transform->Delete();
	transformFilter->Delete();
	SourceMapper->Delete();
	//SourceActor->Delete();
}

void VTKPrimitives::AddLabel(char *cText, double *dCoords, double *dRGB, double dOpacity, double dscale)
{
	vtkVectorText *text = vtkVectorText::New();
	vtkPolyDataMapper *Mapper = vtkPolyDataMapper::New();
	vtkFollower *Actor = vtkFollower::New();
	text->SetText(cText);
	Mapper->SetInput(text->GetOutput());
	Actor->SetMapper(Mapper);
	Actor->SetScale(dscale);
	Actor->SetCamera(ren->GetActiveCamera());
    Actor->GetProperty()->SetColor(dRGB); 
	Actor->GetProperty()->SetOpacity(dOpacity);
	Actor->SetPosition(dCoords);
    
	ren->AddActor(Actor);
	ActorColl->AddItem(Actor);

	//Actor->Delete();
	Mapper->Delete();
	text->Delete();
}

void VTKPrimitives::AddRotationalPoly(double *dCoords, unsigned int uiQtyCoords, double *fRotAxis, double *dRGB, double dOpacity, int iResolution)
{//complete, noch nicht ausgiebig getestet!!!
	unsigned int i=0;
	double start[3]={0,0,0},vector[3]={0,0,0},origin[3]={0,0,0},Footpoint[3]={0,0,0},Basepoint[3]={0,0,0},point[3]={0,0,0};
	double *radius,*level;

	vtkPoints *points = vtkPoints::New();
	vtkCellArray *poly = vtkCellArray::New();
	vtkPolyData *profile = vtkPolyData::New();
	vtkRotationalExtrusionFilter *extrude = vtkRotationalExtrusionFilter::New();
	vtkTransform *transform = vtkTransform::New();
	vtkTransformPolyDataFilter *transformFilter = vtkTransformPolyDataFilter::New();
	vtkPolyDataMapper *Mapper = vtkPolyDataMapper::New();
	vtkActor *Actor = vtkActor::New();

	vector[0]=fRotAxis[1]-fRotAxis[0];
	vector[1]=fRotAxis[3]-fRotAxis[2];
	vector[2]=fRotAxis[5]-fRotAxis[4];
	start[0]=fRotAxis[0];
	start[1]=fRotAxis[2];
	start[2]=fRotAxis[4];

	DistancePointLine(origin,start,vector,Footpoint);

	//debugging
//	for (i=0; i<6; i++) fprintf(stderr,"\n Achse: [%i]: %f",i,fRotAxis[i]);
//	fprintf(stderr,"\n Richtungsvektor: %f %f %f",vector[0],vector[1],vector[2]);
//	fprintf(stderr,"\n Fusspunkt: %f %f %f",Footpoint[0],Footpoint[1],Footpoint[2]);
//	fprintf(stderr,"\n Start: %f %f %f",start[0],start[1],start[2]);

	radius=(double *)calloc(uiQtyCoords,sizeof(double));
	level=(double *)calloc(uiQtyCoords,sizeof(double));

	poly->InsertNextCell(uiQtyCoords+1);

	for (i=0;i<uiQtyCoords;i++)
	{
		for (int j=0; j<3; j++) point[j]=dCoords[uiQtyCoords*j+i];
		radius[i]=DistancePointLine(point,start,vector,Basepoint);
		level[i]=DistancePointPoint(Basepoint,Footpoint);
		points->InsertPoint(i,radius[i],0,level[i]);
		poly->InsertCellPoint(i);
	}
	poly->InsertCellPoint(0);
	profile->SetPoints(points);
	profile->SetLines(poly);
	extrude->SetInput(profile);
	extrude->SetResolution(iResolution);
	extrude->SetAngle(360.0);


	double alpha=VectorAngel(vector[0],sqrt(vector[1]*vector[1]+vector[2]*vector[2]),0,0,1,0);
	double beta=VectorAngel(0,vector[1],vector[2],0,1,0);
	if (fRotAxis[0]>0) alpha=-alpha;
	if (fRotAxis[2]<0) beta=-beta;
	fprintf(stderr," alpha: %f  beta: %f \n",alpha,beta);
	transform->Translate(Footpoint);
//	transform->RotateX(-beta);
//	transform->RotateZ(alpha);
//	transform->RotateWXYZ(-90,1,0,0);

	transform->RotateWXYZ(beta,1,0,0);
	transform->RotateWXYZ(alpha,0,0,1);
	transform->RotateWXYZ(-90,1,0,0);

	transformFilter->SetInput(extrude->GetOutput());
	transformFilter->SetTransform(transform);

	Mapper->SetInput(transformFilter->GetOutput());
	Actor->SetMapper(Mapper);
	Actor->GetProperty()->SetColor(dRGB);
	Actor->GetProperty()->SetOpacity(dOpacity);

	ActorColl->AddItem(Actor);
	ren->AddActor(Actor);

	free(level);
	free(radius);
	points->Delete();
	poly->Delete();
	profile->Delete();
	extrude->Delete();
	transform->Delete();
	transformFilter->Delete();
	Mapper->Delete();
	//Actor->Delete();
}

void VTKPrimitives::AddRotationalSolid(double *dPoint, double fRadius, double *fRotAxis, double *dRGB, double dOpacity, int iResolution)
{
	vtkPoints *points = vtkPoints::New();
	vtkCellArray *poly = vtkCellArray::New();
	vtkPolyData *profile = vtkPolyData::New();
	vtkRotationalExtrusionFilter *extrude = vtkRotationalExtrusionFilter::New();
	vtkTransform *transform = vtkTransform::New();
	vtkTransformPolyDataFilter *transformFilter = vtkTransformPolyDataFilter::New();
	vtkPolyDataMapper *Mapper = vtkPolyDataMapper::New();
	vtkActor *Actor = vtkActor::New();

	double vector[3]={0,0,0},start[3]={0,0,0},Footpoint[3]={0,0,0};
//	for (int i=0; i<6; i++) fprintf(stderr,"\n fRotAxis[%i]: %f ",i, fRotAxis[i]); //debugging
	vector[0]=fRotAxis[1]-fRotAxis[0];
	vector[1]=fRotAxis[3]-fRotAxis[2];
	vector[2]=fRotAxis[5]-fRotAxis[4];
	start[0]=fRotAxis[0];
	start[1]=fRotAxis[2];
	start[2]=fRotAxis[4];
//	for (int i=0; i<3; i++) fprintf(stderr,"\n Coords[%i]: %f start[%i]: %f vector[%i]: %f ",i,dCoords[i],i,start[i],i,vector[i]); //debugging
	double dSolidRadius=DistancePointLine(dPoint,start,vector,Footpoint);

//	fprintf(stderr,"\n dSolidRadius: %f Footpoint: %f %f %f\n",dSolidRadius,Footpoint[0],Footpoint[1],Footpoint[2]); //debugging

	poly->InsertNextCell(iResolution+1);
	for (int i=0; i<iResolution; i++) 
	{
		points->InsertPoint(i,dSolidRadius+fRadius*cos(i*2*PI/iResolution),0,fRadius*sin(i*2*PI/iResolution));
		poly->InsertCellPoint(i);
	}
	poly->InsertCellPoint(0);
	profile->SetPoints(points);
	profile->SetLines(poly);

	extrude->SetInput(profile);
	extrude->SetResolution(iResolution);
	extrude->SetAngle(360.0);



	double alpha=VectorAngel(vector[0],sqrt(vector[1]*vector[1]+vector[2]*vector[2]),0,0,1,0);
	double beta=VectorAngel(0,vector[1],vector[2],0,1,0);
	if (vector[0]>0) alpha=-alpha;
	if (vector[2]<0) beta=-beta;
	transform->Translate(Footpoint);
	transform->RotateWXYZ(beta,1,0,0);
	transform->RotateWXYZ(alpha,0,0,1);
	transform->RotateWXYZ(-90,1,0,0);

	transformFilter->SetInput(extrude->GetOutput());
	transformFilter->SetTransform(transform);

	Mapper->SetInput(transformFilter->GetOutput());
	Actor->SetMapper(Mapper);
	Actor->GetProperty()->SetColor(dRGB);
	Actor->GetProperty()->SetOpacity(dOpacity);

	ActorColl->AddItem(Actor);
	ren->AddActor(Actor);

	points->Delete();
	poly->Delete();
	profile->Delete();
	extrude->Delete();
	transform->Delete();
	transformFilter->Delete();
	Mapper->Delete();
	//Actor->Delete();
}


void VTKPrimitives::AddSurface(double *dCoords, unsigned int uiQtyCoords, double *dRGB, double dOpacity)
{//complete
	unsigned int i=0;
	vtkPoints *points = vtkPoints::New();
	vtkCellArray *poly = vtkCellArray::New();
	vtkPolyData *profile = vtkPolyData::New();
	vtkPolyDataMapper *Mapper = vtkPolyDataMapper::New();
	vtkActor *Actor = vtkActor::New();
	for (i=0; i<uiQtyCoords;i++) points->InsertPoint(i,dCoords[3*i],dCoords[3*i+1],dCoords[3*i+2]);
	for (i=0; i<uiQtyCoords;)
	{
		poly->InsertNextCell(3);
		poly->InsertCellPoint(i++);
		poly->InsertCellPoint(i++);
		poly->InsertCellPoint(i++);
	}
	profile->SetPoints(points);
	profile->SetPolys(poly);
	Mapper->SetInput(profile);
	Actor->SetMapper(Mapper);
	Actor->GetProperty()->SetColor(dRGB);
	Actor->GetProperty()->SetOpacity(dOpacity);


	ActorColl->AddItem(Actor);
	ren->AddActor(Actor);

	points->Delete();
	poly->Delete();
	profile->Delete();
	Mapper->Delete();
	//Actor->Delete();

}



void VTKPrimitives::AddSTLObject(char *Filename, double *dCenter, double *dRGB, double dOpacity)
{ //complete??
	vtkSTLReader *part = vtkSTLReader::New();
	part->SetFileName(Filename);
	vtkPolyDataMapper *partMapper = vtkPolyDataMapper::New();
	partMapper->SetInput(part->GetOutput());
	partMapper->ScalarVisibilityOff();
	vtkActor *partActor = vtkActor::New();
	partActor->SetMapper(partMapper);
	partActor->GetProperty()->SetColor(dRGB);
	partActor->GetProperty()->SetOpacity(dOpacity);
	partActor->SetPosition(dCenter);
	ren->AddActor(partActor);
	ActorColl->AddItem(partActor);
	part->Delete();
	partMapper->Delete();
	//partActor->Delete();
}

void VTKPrimitives::SetOpacity2All(double opacity)
{
	ActorColl->InitTraversal();
	vtkActor* act=NULL;
	while (act=ActorColl->GetNextActor())
	{
		act->GetProperty()->SetOpacity(opacity);
	}
}

double VTKPrimitives::VectorAngel(double dV1_1, double dV1_2, double dV1_3, double dV2_1, double dV2_2, double dV2_3)
{
	double angel=0,dV1L,dV2L;
	double scalarP;
	dV1L=sqrt(dV1_1*dV1_1+dV1_2*dV1_2+dV1_3*dV1_3);
	dV2L=sqrt(dV2_1*dV2_1+dV2_2*dV2_2+dV2_3*dV2_3);
	scalarP=dV1_1*dV2_1+dV1_2*dV2_2+dV1_3*dV2_3;
	if ((dV1L*dV2L)==0) return 0;
	angel=scalarP/(dV1L*dV2L);
	if (angel>1) angel=0.0;
	else if (angel<-1) angel=180.0;
	else angel=acos(angel)*180/PI;
	return angel;
}


double VTKPrimitives::DistancePointLine(double *dpoint,double *dstart,double *dvector, double *dFootpoint)
{
	double dpos=0;
	dpos= ( (dpoint[0]-dstart[0])*dvector[0]+(dpoint[1]-dstart[1])*dvector[1]+(dpoint[2]-dstart[2])*dvector[2] ) /(dvector[0]*dvector[0]+dvector[1]*dvector[1]+dvector[2]*dvector[2]);
	for (int i=0; i<3; i++) dFootpoint[i]=dstart[i]+dpos*dvector[i];
	return sqrt( (dpoint[0]-dFootpoint[0])*(dpoint[0]-dFootpoint[0]) + (dpoint[1]-dFootpoint[1])*(dpoint[1]-dFootpoint[1]) + (dpoint[2]-dFootpoint[2])*(dpoint[2]-dFootpoint[2]) );
}

double VTKPrimitives::DistancePointPoint(double *dpoint1, double *dpoint2)
{
	return sqrt( (dpoint1[0]-dpoint2[0])*(dpoint1[0]-dpoint2[0]) + (dpoint1[1]-dpoint2[1])*(dpoint1[1]-dpoint2[1]) + (dpoint1[2]-dpoint2[2])*(dpoint1[2]-dpoint2[2]) );
}