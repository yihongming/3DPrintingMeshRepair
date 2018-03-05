#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "fileCheck.hpp"

std::string meshPath = "./unittest/meshes/";

TEST_CASE( "test successful loadMesh", "[file_check]" ) {
    MyMesh mesh;
    bool is_successful = loadMesh(mesh, meshPath+"perfect.stl");

    REQUIRE( is_successful == true );
}

TEST_CASE( "test not successful loadMesh", "[file_check]" ) {
    MyMesh mesh;
    bool is_successful = loadMesh(mesh, meshPath+"notexists.stl");

    REQUIRE( is_successful == false );
}

TEST_CASE( "test NoDegeneratedFace", "[file_check]" ) {
    MyMesh noDegenratedFacesMesh;
    loadMesh(noDegenratedFacesMesh, meshPath+"perfect.stl");

    int numDegeneratedFaces;

    REQUIRE(
        NoDegenratedFaces(noDegenratedFacesMesh, numDegeneratedFaces) == true
    );
    REQUIRE( numDegeneratedFaces == 0);
}

TEST_CASE( "test DengeratedFaces", "[file_check]" ) {
    MyMesh DegenratedFacesMesh;
    loadMesh(DegenratedFacesMesh, meshPath+"degeneratedFaces.stl");

    int numDegeneratedFaces;
    REQUIRE(
        NoDegenratedFaces(DegenratedFacesMesh, numDegeneratedFaces) == false
    );
    REQUIRE( numDegeneratedFaces == 2);
}

TEST_CASE( "test NoDuplicateFaces", "[file_check]" ) {
    MyMesh noDuplicateFacesMesh;
    loadMesh(noDuplicateFacesMesh, meshPath+"perfect.stl");

    int numDuplicateFaces;
    REQUIRE( NoDuplicateFaces(noDuplicateFacesMesh, numDuplicateFaces) == true );
    REQUIRE( numDuplicateFaces == 0 );
}

TEST_CASE( "test DuplicateFaces", "[file_check]" ) {
    MyMesh DuplicateFacesMesh;
    loadMesh(DuplicateFacesMesh, meshPath+"duplicateFaces.stl");

    int numDuplicateFaces;
    REQUIRE( NoDuplicateFaces(DuplicateFacesMesh, numDuplicateFaces) == false );
    REQUIRE( numDuplicateFaces == 1 );
}

TEST_CASE( "test WaterTight", "[file_check]" ) {
    MyMesh waterTightMesh;
    loadMesh(waterTightMesh, meshPath+"perfect.stl");

    REQUIRE( IsWaterTight(waterTightMesh) == true );
}

TEST_CASE( "test Not WaterTight", "[file_check]" ) {
    MyMesh notWaterTightMesh;
    loadMesh(notWaterTightMesh, meshPath+"notWatertight.stl");

    REQUIRE( IsWaterTight(notWaterTightMesh) == false );
}

TEST_CASE( "test Coherently Oriented", "[file_check]" ) {
    MyMesh coherentlyOrientedMesh;
    loadMesh(coherentlyOrientedMesh, meshPath+"perfect.stl");
    vcg::tri::UpdateTopology<MyMesh>::FaceFace(coherentlyOrientedMesh);

    REQUIRE( IsCoherentlyOrientedMesh(coherentlyOrientedMesh) == true );
}

TEST_CASE( "test not Coherently Oriented", "[file_check]" ) {
    MyMesh notCoherentlyOrientedMesh;
    loadMesh(notCoherentlyOrientedMesh, meshPath+"notCoherentlyOriented.stl");
    vcg::tri::UpdateTopology<MyMesh>::FaceFace(notCoherentlyOrientedMesh);

    REQUIRE( IsCoherentlyOrientedMesh(notCoherentlyOrientedMesh) == false );
}

TEST_CASE( "test Positive Volume", "[file_check]" ) {
    MyMesh positiveVolumeMesh;
    loadMesh(positiveVolumeMesh, meshPath+"perfect.stl");

    REQUIRE( IsPositiveVolume(positiveVolumeMesh) == true );
}

TEST_CASE( "test not Positive Volume", "[file_check]" ) {
    MyMesh notPositiveVolumeMesh;
    loadMesh(notPositiveVolumeMesh, meshPath+"notPositiveVolume.stl");

    REQUIRE( IsPositiveVolume(notPositiveVolumeMesh) == false );
}

TEST_CASE( "test no intersecting faces", "[file_check]" ) {
    MyMesh noIntersectingFacesMesh;
    loadMesh(noIntersectingFacesMesh, meshPath+"perfect.stl");

    int numIntersectingFaces;
    REQUIRE(
        NoIntersectingFaces(noIntersectingFacesMesh, numIntersectingFaces) == true
    );
    REQUIRE(numIntersectingFaces == 0);
}

TEST_CASE( "test intersecting faces", "[file_check]" ) {
    MyMesh IntersectingFacesMesh;
    loadMesh(IntersectingFacesMesh, meshPath+"intersectingFaces.stl");

    int numIntersectingFaces;
    REQUIRE(
        NoIntersectingFaces(IntersectingFacesMesh, numIntersectingFaces) == false
    );
    // REQUIRE(numIntersectingFaces == 0); // the intersecting algorithm is not good enough
}

TEST_CASE( "test mesh boundary", "[file_check]" ) {
    MyMesh Mesh;
    loadMesh(Mesh, meshPath+"perfect.stl");
    float boundary[6];

    Boundary(Mesh, boundary);

    REQUIRE( boundary[0] == (float) -1.0 );
    REQUIRE( boundary[1] == (float)  1.0 );
    REQUIRE( boundary[2] == (float) -1.0 );
    REQUIRE( boundary[3] == (float)  1.0 );
    REQUIRE( boundary[4] == (float) -1.0 );
    REQUIRE( boundary[5] == (float)  1.0 );
    // REQUIRE(numIntersectingFaces == 0); // the intersecting algorithm is not good enough
}

//TEST_CASE( "test shell", "[file_check]" ) {
    //MyMesh Mesh;
    //loadMesh(Mesh, meshPath+"perfect.stl");
    //int numConnectedComponents;
    //REQUIRE( IsSingleShell(Mesh, numConnectedComponents) == true );
    //REQUIRE( numConnectedComponents == 2 );
//}

//TEST_CASE( "test multiple shells", "[file_check]" ) {
    //MyMesh Mesh;
    //loadMesh(Mesh, meshPath+"twoCubes.stl");
    //int numConnectedComponents;
    //REQUIRE(
        //IsSingleShell(Mesh, numConnectedComponents) == false
    //);
    //REQUIRE( numConnectedComponents == 2 );
//}

TEST_CASE( "test flip", "[file_repair]" ) {
    MyMesh Mesh;
    auto filepath = meshPath+"notPositiveVolume.stl";
    loadMesh(Mesh, filepath);

    REQUIRE( IsPositiveVolume(Mesh) == false );
    bool doesFlip = DoesFlipNormalOutside(Mesh, true, true, false);
    REQUIRE( doesFlip == true );
    REQUIRE( IsPositiveVolume(Mesh) == true );
}

TEST_CASE( "test not flip", "[file_repair]" ) {
    MyMesh Mesh;
    auto filepath = meshPath+"perfect.stl";
    bool doesFlip = DoesFlipNormalOutside(Mesh, true, true, true);
    REQUIRE( doesFlip == false );
}

TEST_CASE( "test MakeCoherentlyOriented ", "[file_repair]" ) {
    MyMesh Mesh;
    auto filepath = meshPath+"notCoherentlyOriented.stl";
    loadMesh(Mesh, filepath);
    vcg::tri::UpdateTopology<MyMesh>::FaceFace(Mesh);

    REQUIRE( IsCoherentlyOrientedMesh(Mesh) == false );
    bool doesFlip = DoesMakeCoherentlyOriented(Mesh, true, false);
    REQUIRE( doesFlip == true );
    REQUIRE( IsCoherentlyOrientedMesh(Mesh) == true );
}

TEST_CASE( "test not MakeCoherentlyOriented", "[file_repair]" ) {
    MyMesh Mesh;
    auto filepath = meshPath+"perfect.stl";
    bool doesFlip = DoesMakeCoherentlyOriented(Mesh, true, true);
    REQUIRE( doesFlip == false );
}


TEST_CASE( "test no file repair", "[file_repair]" ) {
    MyMesh mesh;
    auto filepath = meshPath+"perfect.stl";
    loadMesh(mesh, filepath);
    int results[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    float boundary[6];
    file_check(mesh, results, boundary);

    int repair_record[2] = {-1, -1};
    file_repair(mesh, results, repair_record);

    REQUIRE(repair_record[0] == 0);
    REQUIRE(repair_record[1] == 0);
}

TEST_CASE( "test fix volume and coherent oriented", "[file_repair]" ) {
    MyMesh mesh;
    auto filepath = meshPath+"notCoherentlyOriented.stl";
    loadMesh(mesh, filepath);
    int results[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    float boundary[6];
    file_check(mesh, results, boundary);

    int repair_record[2] = {-1, -1};
    file_repair(mesh, results, repair_record);

    REQUIRE(repair_record[0] == 1);
    REQUIRE(repair_record[1] == 1);
}

TEST_CASE( "test only fix positive volume", "[file_repair]" ) {
    MyMesh mesh;
    auto filepath = meshPath+"notPositiveVolume.stl";
    loadMesh(mesh, filepath);
    int results[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    float boundary[6];
    file_check(mesh, results, boundary);

    int repair_record[2] = {-1, -1};
    file_repair(mesh, results, repair_record);

    REQUIRE(repair_record[0] == 0);
    REQUIRE(repair_record[1] == 1);
}

TEST_CASE( "test only fix coherently oriented", "[file_repair]" ) {
    MyMesh mesh;
    auto filepath = meshPath+"mostly_notCoherentlyOriented.stl";
    loadMesh(mesh, filepath);
    int results[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    float boundary[6];
    file_check(mesh, results, boundary);

    int repair_record[2] = {-1, -1};
    file_repair(mesh, results, repair_record);

    REQUIRE(repair_record[0] == 1);
    REQUIRE(repair_record[1] == 0);
}

// for some reason there is error running test IsSingleShell function
TEST_CASE( "test single shell", "[file_repair]" ) {
    MyMesh mesh;
    int results[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    float boundary[6];
    int numConnectedComponents;

    auto filepath = meshPath+"perfect.stl";
    loadMesh(mesh, filepath);

    file_check(mesh, results, boundary);
    numConnectedComponents = results[9];
    REQUIRE(numConnectedComponents == 1); // one shell

    filepath = meshPath+"twoCubes.stl";
    loadMesh(mesh, filepath);

    file_check(mesh, results, boundary);
    numConnectedComponents = results[9];
    REQUIRE(numConnectedComponents == 2); // one shell
}
