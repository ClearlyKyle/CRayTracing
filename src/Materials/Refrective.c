#include "Material.h"

#include "../Lights/Lights.h"

// Forward declarations
static vec3 Material_Refractive_Compute_Translucency(Objects objects, const Lights lights, const size_t current_object_index, vec3 *const int_point, vec3 *const local_normal, const Ray *indicent_ray);
static vec3 Material_Refractive_Compute_Specular(const Material mat, Objects objects, const Lights lights, vec3 *const int_point, vec3 *const local_normal, const Ray *camera_ray);

vec3 Material_Refrective_Compute_Colour(Objects      objects,
                                        const Lights lights,
                                        const size_t current_object_index,
                                        vec3 *const  int_point,
                                        vec3 *const  local_normal,
                                        const Ray   *camera_ray)
{
    const Material mat = *objects.shapes[current_object_index].mat;
    const Texture  tex = mat.texture;

    // Define the initial material colors.
    vec3 mat_colour = VEC3_INIT_ZERO;
    vec3 ref_colour = VEC3_INIT_ZERO;
    vec3 dif_colour = VEC3_INIT_ZERO;
    vec3 spc_colour = VEC3_INIT_ZERO;
    vec3 trn_colour = VEC3_INIT_ZERO;

    // Compute the diffuse component.
    if (mat.has_texture)
    {
        const vec4 texture_colour = Texture_Get_Colour(tex, objects.shapes[current_object_index].uv_coordinates);
        dif_colour                = Material_Base_Compute_Diffuse_Colour(objects, lights, current_object_index, int_point, local_normal, (vec3){texture_colour.x, texture_colour.y, texture_colour.z});
    }
    else
    {
        dif_colour = Material_Base_Compute_Diffuse_Colour(objects, lights, current_object_index, int_point, local_normal, mat.base_colour);
    }
    // Compute the reflection component.
    if (mat.reflectivity > 0.0)
        ref_colour = Material_Base_Compute_Reflection_Colour(objects, lights, current_object_index, int_point, local_normal, camera_ray, mat.base_colour);

    // Combine reflection and diffuse components.
    mat_colour.x = (ref_colour.x * mat.reflectivity) + (dif_colour.x * (1.0 - mat.reflectivity));
    mat_colour.y = (ref_colour.y * mat.reflectivity) + (dif_colour.y * (1.0 - mat.reflectivity));
    mat_colour.z = (ref_colour.z * mat.reflectivity) + (dif_colour.z * (1.0 - mat.reflectivity));

    // Compute the refractive component.
    if (mat.translucency > 0.0)
        trn_colour = Material_Refractive_Compute_Translucency(objects, lights, current_object_index, int_point, local_normal, camera_ray);

    // And combine with the current color.
    mat_colour.x = (trn_colour.x * mat.translucency) + (mat_colour.x * (1.0 - mat.translucency));
    mat_colour.y = (trn_colour.y * mat.translucency) + (mat_colour.y * (1.0 - mat.translucency));
    mat_colour.z = (trn_colour.z * mat.translucency) + (mat_colour.z * (1.0 - mat.translucency));

    // And compute the specular component.
    if (mat.shininess > 0.0)
        spc_colour = Material_Refractive_Compute_Specular(mat, objects, lights, int_point, local_normal, camera_ray);

    // Add the specular component to the final color.
    mat_colour = vec3_add(mat_colour, spc_colour);

    return mat_colour;
}

static Ray _Compute_Refracted_Ray(const Ray indicent_ray, vec3 normal, vec3 int_point, const double ior)
{
    // Compute the refracted vector.
    vec3 p          = vec3_normalise(indicent_ray.lab);
    vec3 tmp_normal = normal;

    const double r = 1.0 / ior;
    double       c = -vec3_dot(tmp_normal, p);

    if (c < 0.0)
    {
        tmp_normal = vec3_mul_scal(tmp_normal, -1.0);
        c          = -vec3_dot(tmp_normal, p);
    }

    // vec3 refracted_vector = r * p + (r * c - sqrtf(1.0 - pow(r, 2.0) * (1.0 - pow(c, 2.0)))) * tempNormal;
    const double brackets         = (r * c - sqrt(1.0 - pow(r, 2.0) * (1.0 - pow(c, 2.0))));
    const vec3   right_of_mul     = vec3_mul_scal(tmp_normal, brackets);
    const vec3   left_of_add      = vec3_mul_scal(p, r);
    const vec3   refracted_vector = vec3_add(left_of_add, right_of_mul);

    // Construct the refracted ray.
    const vec3 ray_p1        = vec3_add(int_point, vec3_mul_scal(refracted_vector, 0.01));
    const vec3 ray_p2        = vec3_add(int_point, refracted_vector);
    const Ray  refracted_ray = Ray_Init(ray_p1, ray_p2);

    return refracted_ray;
}

static vec3 Material_Refractive_Compute_Translucency(Objects      objects,
                                                     const Lights lights,
                                                     const size_t current_object_index,
                                                     vec3 *const  int_point,
                                                     vec3 *const  local_normal,
                                                     const Ray   *indicent_ray)
{
    double ior = 1.0;

    vec3 trn_colour = VEC3_INIT_ZERO;

    //// Compute the refracted vector.
    // vec3 p          = vec3_normalise(indicent_ray->lab);
    // vec3 tmp_normal = *local_normal;

    // const double r = 1.0 / ior;
    // double       c = -vec3_dot(tmp_normal, p);

    // if (c < 0.0)
    //{
    //     tmp_normal = vec3_mul_scal(tmp_normal, -1.0);
    //     c          = -vec3_dot(tmp_normal, p);
    // }

    //// vec3 refracted_vector = r * p + (r * c - sqrtf(1.0 - pow(r, 2.0) * (1.0 - pow(c, 2.0)))) * tempNormal;
    // const double brackets         = (r * c - sqrtf(1.0 - pow(r, 2.0) * (1.0 - pow(c, 2.0))));
    // const vec3   right_of_mul     = vec3_mul_scal(tmp_normal, brackets);
    // const vec3   left_of_add      = vec3_mul_scal(p, r);
    // const vec3   refracted_vector = vec3_add(left_of_add, right_of_mul);

    //// Construct the refracted ray.
    // const vec3 ray_p1        = vec3_add(*int_point, vec3_mul_scal(refracted_vector, 0.01));
    // const vec3 ray_p2        = vec3_add(*int_point, refracted_vector);
    // const Ray  refracted_ray = Ray_Init(ray_p1, ray_p2);

    const Ray refracted_ray = _Compute_Refracted_Ray(*indicent_ray, *local_normal, *int_point, ior);

    // Test for secondary intersection with this object.
    size_t closest_object;
    vec3   closest_int_point    = VEC3_INIT_ZERO;
    vec3   closest_local_normal = VEC3_INIT_ZERO;
    vec3   closest_local_colour = VEC3_INIT_ZERO;
    vec3   new_int_point        = VEC3_INIT_ZERO;
    vec3   new_local_normal     = VEC3_INIT_ZERO;
    vec3   new_local_colour     = VEC3_INIT_ZERO;

    const bool test = Object_Test_Intersection(&objects.shapes[current_object_index],
                                               refracted_ray,
                                               &new_int_point,
                                               &new_local_normal,
                                               &new_local_colour);

    bool intersection_found = false;
    Ray  final_ray;

    if (test)
    {
        // Compute the refracted vector.
        // const vec3 p2          = vec3_normalise(refracted_ray.lab);
        // vec3       tmp_normal2 = new_local_normal;

        // const double r2 = ior;
        // double       c2 = -vec3_dot(tmp_normal2, p2);
        // if (c2 < 0.0)
        //{
        //     tmp_normal2 = vec3_mul_scal(tmp_normal2, -1.0);
        //     c2          = -vec3_dot(tmp_normal2, p2);
        // }

        //// vec3 refracted_vector2 = r2 * p2 + (r2 * c2 - sqrtf(1.0 - pow(r2, 2.0) * (1.0 - pow(c2, 2.0)))) * tempNormal2;
        // const double brackets2         = (r2 * c2 - sqrtf(1.0 - pow(r2, 2.0) * (1.0 - pow(c2, 2.0))));
        // const vec3   right_of_mul2     = vec3_mul_scal(tmp_normal2, brackets2);
        // const vec3   left_of_add2      = vec3_mul_scal(p2, r2);
        // const vec3   refracted_vector2 = vec3_add(left_of_add2, right_of_mul2);

        //// Construct the refracted ray.
        // const vec3 ray_p1         = vec3_add(*int_point, vec3_mul_scal(refracted_vector, 0.01));
        // const vec3 ray_p2         = vec3_add(*int_point, refracted_vector);
        // const Ray  refracted_ray2 = Ray_Init(ray_p1, ray_p2);

        const Ray refracted_ray2 = _Compute_Refracted_Ray(refracted_ray, new_local_normal, new_int_point, ior);

        // Cast this ray into the scene.
        intersection_found = Material_Cast_Ray(refracted_ray2, objects, current_object_index, &closest_object, &closest_int_point, &closest_local_normal, &closest_local_colour);
        final_ray          = refracted_ray2;
    }
    else
    {
        /* No secondary intersections were found, so continue the original refracted ray. */
        intersection_found = Material_Cast_Ray(refracted_ray, objects, current_object_index, &closest_object, &closest_int_point, &closest_local_normal, &closest_local_colour);
        final_ray          = refracted_ray;
    }

    // Compute the color for closest object.
    vec3 mat_colour = VEC3_INIT_ZERO;
    if (intersection_found)
    {
        // Check if a material has been assigned.
        if (objects.shapes[closest_object].mat)
        {
            mat_colour = Material_Compute_Colour(objects, lights, closest_object, &closest_int_point, &closest_local_normal, &final_ray);
        }
        else
        {
            mat_colour = Material_Base_Compute_Diffuse_Colour(objects,
                                                              lights,
                                                              closest_object,
                                                              &closest_int_point,
                                                              &closest_local_normal,
                                                              objects.shapes[closest_object].base_colour);
        }
    }
    else
    {
        // Leave matColor as it is.
    }

    trn_colour = mat_colour;
    return mat_colour;
}

static vec3 Material_Refractive_Compute_Specular(const Material mat,
                                                 Objects        objects,
                                                 const Lights   lights,
                                                 vec3 *const    int_point,
                                                 vec3 *const    local_normal,
                                                 const Ray     *camera_ray)
{
    double red   = 0.0;
    double green = 0.0;
    double blue  = 0.0;

    // Loop through all of the lights in the scene.
    for (size_t current_light = 0; current_light < lights.count; current_light++)
    {
        /* Check for intersections with all objects in the scene. */
        double intensity = 0.0;

        // Construct a vector pointing from the intersection point to the light.
        const vec3 light_direction = vec3_normalise(vec3_sub(lights.lights[current_light].object.pointLight.location, *int_point));

        // Compute a start point.
        const vec3 start_point = vec3_add(*int_point, vec3_add_scal(light_direction, 0.001));

        // Construct a ray from the point of intersection to the light.
        const Ray light_ray = Ray_Init(start_point, vec3_add(start_point, light_direction));

        /* Loop through all objects in the scene to check if any obstruct light from this source. */
        vec3 poi                = VEC3_INIT_ZERO;
        vec3 poi_normal         = VEC3_INIT_ZERO;
        vec3 poi_colour         = VEC3_INIT_ZERO;
        bool valid_intersection = false;

        for (size_t current_object = 0; current_object < objects.count; current_object++)
        {
            valid_intersection = Object_Test_Intersection(&objects.shapes[current_object],
                                                          light_ray,
                                                          &poi,
                                                          &poi_normal,
                                                          &poi_colour);

            if (valid_intersection)
            {
                break;
            }
        }

        /* If no intersections were found, then proceed with
                computing the specular component. */
        if (!valid_intersection)
        {
            // Compute the reflection vector.
            const vec3 d = light_ray.lab;

            const vec3 r = vec3_normalise(vec3_reflection(d, *local_normal));

            // Compute the dot product.
            const vec3   v           = vec3_normalise(camera_ray->lab);
            const double dot_product = vec3_dot(r, v);

            // Only proceed if the dot product is positive.
            if (dot_product > 0.0)
            {
                intensity = mat.reflectivity * pow(dot_product, mat.shininess);
            }
        }

        red += lights.lights[current_light].object.pointLight.colour.x * intensity;
        green += lights.lights[current_light].object.pointLight.colour.y * intensity;
        blue += lights.lights[current_light].object.pointLight.colour.z * intensity;
    }

    return (vec3){red, green, blue};
}