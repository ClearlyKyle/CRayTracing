#include "Material.h"

vec3 Simple_Material_Compute_Colour(const Material mat,
                                    const Objects  objects,
                                    const Lights   lights,
                                    const size_t   current_object_index,
                                    vec3 *const    int_point,
                                    vec3 *const    local_normal,
                                    const Ray_t   *camera_ray)
{
    // Define the initial material colors.
    vec3 mat_colour = VEC3_INIT_ZERO;
    vec3 ref_colour = VEC3_INIT_ZERO;
    vec3 dif_colour = VEC3_INIT_ZERO;
    vec3 spc_colour = VEC3_INIT_ZERO;

    // Compute the diffuse component.
    dif_colour = Material_Compute_Diffuse_Colour(objects, lights, current_object_index, int_point, local_normal, mat.base_colour);

    // Compute the reflection component.
    if (mat.reflectivity > 0.0)
        ref_colour = Material_Compute_Reflection_Colour(mat, objects, lights, current_object_index, int_point, local_normal, camera_ray, mat.base_colour);

    // Combine reflection and diffuse components.
    mat_colour.x = (ref_colour.x * mat.reflectivity) + (dif_colour.x * (1.0 - mat.reflectivity));
    mat_colour.y = (ref_colour.y * mat.reflectivity) + (dif_colour.y * (1.0 - mat.reflectivity));
    mat_colour.z = (ref_colour.z * mat.reflectivity) + (dif_colour.z * (1.0 - mat.reflectivity));

    // Compute the specular component.
    if (mat.shininess > 0.0)
        spc_colour = SimpleMaterial_Compute_Specular(mat, objects, lights, int_point, local_normal, camera_ray);

    // Add the specular component to the final color.
    mat_colour = vec3_add(mat_colour, spc_colour);

    return mat_colour;
}

vec3 SimpleMaterial_Compute_Specular(const Material mat,
                                     const Objects  objects,
                                     const Lights   lights,
                                     vec3 *const    int_point,
                                     vec3 *const    local_normal,
                                     const Ray_t   *camera_ray)
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
        const vec3 start_point = vec3_add(vec3_mul_scal(light_direction, 0.001), *int_point);

        // Construct a ray from the point of intersection to the light.
        const Ray_t light_ray = Ray_Init(start_point, vec3_add(start_point, light_direction));

        /* Loop through all objects in the scene to check if any obstruct light from this source. */
        vec3 poi        = VEC3_INIT_ZERO;
        vec3 poi_normal = VEC3_INIT_ZERO;
        vec3 poi_colour = VEC3_INIT_ZERO;
        bool validInt   = false;

        for (size_t current_object = 0; current_object < objects.count; current_object++)
        {
            bool validInt = false;

            switch (objects.shapes[current_object].type)
            {
            case SHAPE_SPHERE:
                validInt = Sphere_Test_Intersection(objects.shapes[current_object].object.sphere, &light_ray, &poi, &poi_normal, &poi_colour);
                break;

            case SHAPE_PLANE:
                validInt = Plane_Test_Intersection(objects.shapes[current_object].object.plane, &light_ray, &poi, &poi_normal, &poi_colour);
                break;

            default:
                fprintf(stderr, "THIS SHAPE IS NOT SUPPORTED : %d\n", objects.shapes[current_object].type);
                break;
            }

            if (validInt)
            {
                break;
            }
        }

        /* If no intersections were found, then proceed with computing the specular component. */
        if (!validInt)
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
