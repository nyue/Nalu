/*------------------------------------------------------------------------*/
/*  Copyright 2014 National Renewable Energy Laboratory.                  */
/*  This software is released under the license detailed                  */
/*  in the file, LICENSE, which is located in the top-level Nalu          */
/*  directory structure                                                   */
/*------------------------------------------------------------------------*/

#include "UnitTestAlgorithm.h"
#include "UnitTestUtils.h"
#include "UnitTestKokkosUtils.h"
#include "kernels/UnitTestKernelUtils.h"

#include "Realm.h"

void
TestAlgorithm::fill_mesh(const std::string mesh_spec)
{
  this->declare_fields();

  unit_test_utils::fill_hex8_mesh(mesh_spec, bulk());
  meshPart_ = meta().get_part("block_1");
  coordinates_ = static_cast<const VectorFieldType*>(meta().coordinate_field());
  EXPECT_TRUE(coordinates_ != nullptr);
}

double
TestAlgorithm::field_norm(const ScalarFieldType & field, stk::mesh::Selector* selector)
{

  auto& meta = this->meta();
  auto& bulk = this->bulk();
  auto sel = (selector == nullptr)? meta.locally_owned_part() : *selector;

  return unit_test_utils::field_norm(field, bulk, sel);
}

void
TestTurbulenceAlgorithm::declare_fields()
{
  auto& meta = this->meta();
  auto spatialDim = meta.spatial_dimension();

  density_ = (
    &meta.declare_field<ScalarFieldType>(
      stk::topology::NODE_RANK, "density"));
  viscosity_ = (
    &meta.declare_field<ScalarFieldType>(
      stk::topology::NODE_RANK, "viscosity"));
  tke_ = (
    &meta.declare_field<ScalarFieldType>(
      stk::topology::NODE_RANK, "turbulent_ke"));
  sdr_ = (
    &meta.declare_field<ScalarFieldType>(
      stk::topology::NODE_RANK, "specific_dissipation_rate"));
  minDistance_ = (
    &meta.declare_field<ScalarFieldType>(
      stk::topology::NODE_RANK, "minimum_distance_to_wall"));
  dudx_ = (
    &meta.declare_field<GenericFieldType>(
      stk::topology::NODE_RANK, "dudx"));
  tvisc_ = (
    &meta.declare_field<ScalarFieldType>(
      stk::topology::NODE_RANK, "turbulent_viscosity"));
  maxLengthScale_ = (
    &meta.declare_field<ScalarFieldType>(
      stk::topology::NODE_RANK, "sst_max_length_scale"));
  fOneBlend_ = (
    &meta.declare_field<ScalarFieldType>(
      stk::topology::NODE_RANK, "sst_f_one_blending"));
  evisc_ = (
     &meta.declare_field<ScalarFieldType>(
       stk::topology::NODE_RANK, "effective_viscosity"));
  evisc_ = (
     &meta.declare_field<ScalarFieldType>(
       stk::topology::NODE_RANK, "effective_viscosity"));
  dualNodalVolume_ = (
     &meta.declare_field<ScalarFieldType>(
       stk::topology::NODE_RANK, "dual_nodal_volume"));
  dkdx_ = (
     &meta.declare_field<VectorFieldType>(
       stk::topology::NODE_RANK, "dkdx"));
  dwdx_ = (
     &meta.declare_field<VectorFieldType>(
       stk::topology::NODE_RANK, "dwdx"));
  dhdx_ = (
     &meta.declare_field<VectorFieldType>(
       stk::topology::NODE_RANK, "dhdx"));
  
  specificHeat_ = (
     &meta.declare_field<ScalarFieldType>(
       stk::topology::NODE_RANK, "specific_heat"));

  stk::mesh::put_field(*density_, meta.universal_part(), 1);
  stk::mesh::put_field(*viscosity_, meta.universal_part(), 1);
  stk::mesh::put_field(*tke_, meta.universal_part(), 1);
  stk::mesh::put_field(*sdr_, meta.universal_part(), 1);
  stk::mesh::put_field(*minDistance_, meta.universal_part(), 1);
  stk::mesh::put_field(*dudx_, meta.universal_part(), spatialDim*spatialDim);
  stk::mesh::put_field(*tvisc_, meta.universal_part(), 1);
  stk::mesh::put_field(*maxLengthScale_, meta.universal_part(), 1);
  stk::mesh::put_field(*fOneBlend_, meta.universal_part(), 1);
  stk::mesh::put_field(*evisc_, meta.universal_part(), 1);
  stk::mesh::put_field(*dualNodalVolume_, meta.universal_part(), 1);
  stk::mesh::put_field(*dkdx_, meta.universal_part(), spatialDim);
  stk::mesh::put_field(*dwdx_, meta.universal_part(), spatialDim);
  stk::mesh::put_field(*dhdx_, meta.universal_part(), spatialDim);
  stk::mesh::put_field(*specificHeat_, meta.universal_part(), 1);
}

void
TestTurbulenceAlgorithm::fill_mesh_and_init_fields(const std::string mesh_spec)
{
  fill_mesh(mesh_spec);

  auto& bulk = this->bulk();

  unit_test_kernel_utils::density_test_function(bulk, *coordinates_, *density_);
  stk::mesh::field_fill(0.2, *viscosity_);
  unit_test_kernel_utils::tke_test_function(bulk, *coordinates_, *tke_);
  unit_test_kernel_utils::sdr_test_function(bulk, *coordinates_, *sdr_);
  unit_test_kernel_utils::minimum_distance_to_wall_test_function(bulk, *coordinates_, *minDistance_);
  unit_test_kernel_utils::dudx_test_function(bulk, *coordinates_, *dudx_);
  unit_test_kernel_utils::turbulent_viscosity_test_function(bulk, *coordinates_, *tvisc_);
  stk::mesh::field_fill(0.5, *maxLengthScale_);
  unit_test_kernel_utils::sst_f_one_blending_test_function(bulk, *coordinates_, *fOneBlend_);
  stk::mesh::field_fill(0.0, *evisc_);
  stk::mesh::field_fill(0.2, *dualNodalVolume_);
  unit_test_kernel_utils::dkdx_test_function(bulk, *coordinates_, *dkdx_);
  unit_test_kernel_utils::dwdx_test_function(bulk, *coordinates_, *dwdx_);
  unit_test_kernel_utils::dhdx_test_function(bulk, *coordinates_, *dhdx_);
  stk::mesh::field_fill(1000.0, *specificHeat_);
}
