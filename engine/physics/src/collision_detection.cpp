#include "engine/physics/collision_detection.hpp"

#include <algorithm>
#include <limits>

#include "engine/core/math.hpp"

EGE_NAMESPACE_BEGIN

namespace
{
constexpr f32 UNIT_ONE = 1.0F;
constexpr f32 UNIT_NEGATIVE_ONE = -1.0F;
constexpr f32 UNIT_ZERO = 0.0F;
constexpr f32 HALF_FACTOR = 0.5F;

constexpr f32 CLAMP_MAX_VALUE = 1.0F;
constexpr f32 INITIAL_DIRECTION_X = 1.0F;
constexpr f32 INITIAL_DIRECTION_Y = 0.0F;
constexpr u32 MAX_GJK_ITERATIONS = 32U;

[[nodiscard]] auto projectPolygon(const std::vector<Vec2>& vertices, const Vec2& axis) -> std::pair<f32, f32>
{
  f32 minProjection = std::numeric_limits<f32>::max();
  f32 maxProjection = std::numeric_limits<f32>::lowest();

  for (const auto& vertex : vertices) {
    f32 projection = glm::dot(vertex, axis);
    minProjection = math::min(minProjection, projection);
    maxProjection = math::max(maxProjection, projection);
  }

  return {minProjection, maxProjection};
}
} // namespace

[[nodiscard]] auto CollisionDetection::detect(Entity entityAlpha, Entity entityBeta, World& world) -> CollisionResult
{
  CollisionResult result;
  result.entityA = entityAlpha;
  result.entityB = entityBeta;

  auto* transformAlpha = world.getComponent<Transform>(entityAlpha);
  auto* transformBeta = world.getComponent<Transform>(entityBeta);
  auto* colliderAlpha = world.getComponent<Collider>(entityAlpha);
  auto* colliderBeta = world.getComponent<Collider>(entityBeta);

  if (transformAlpha == nullptr || transformBeta == nullptr || colliderAlpha == nullptr || colliderBeta == nullptr) {
    return result;
  }

  auto typeAlpha = colliderAlpha->getType();
  auto typeBeta = colliderBeta->getType();

  if (typeAlpha == ColliderType::BOX && typeBeta == ColliderType::BOX) {
    return boxVsBox(*transformAlpha, *colliderAlpha, *transformBeta, *colliderBeta);
  }

  if (typeAlpha == ColliderType::CIRCLE && typeBeta == ColliderType::CIRCLE) {
    return circleVsCircle(*transformAlpha, *colliderAlpha, *transformBeta, *colliderBeta);
  }

  if ((typeAlpha == ColliderType::BOX && typeBeta == ColliderType::CIRCLE) ||
      (typeAlpha == ColliderType::CIRCLE && typeBeta == ColliderType::BOX)) {
    if (typeAlpha == ColliderType::BOX) {
      return boxVsCircle(*transformAlpha, *colliderAlpha, *transformBeta, *colliderBeta);
    }

    auto resultLocal = boxVsCircle(*transformBeta, *colliderBeta, *transformAlpha, *colliderAlpha);
    std::swap(resultLocal.entityA, resultLocal.entityB);
    resultLocal.normal = -resultLocal.normal;
    return resultLocal;
  }

  if (typeAlpha == ColliderType::POLYGON && typeBeta == ColliderType::POLYGON) {
    return polygonVsPolygon(*transformAlpha, *colliderAlpha, *transformBeta, *colliderBeta);
  }

  if ((typeAlpha == ColliderType::EDGE && typeBeta == ColliderType::CIRCLE) ||
      (typeAlpha == ColliderType::CIRCLE && typeBeta == ColliderType::EDGE)) {
    if (typeAlpha == ColliderType::EDGE) {
      return edgeVsCircle(*transformAlpha, *colliderAlpha, *transformBeta, *colliderBeta);
    }

    auto resultLocal = edgeVsCircle(*transformBeta, *colliderBeta, *transformAlpha, *colliderAlpha);
    std::swap(resultLocal.entityA, resultLocal.entityB);
    resultLocal.normal = -resultLocal.normal;
    return resultLocal;
  }

  return result;
}

[[nodiscard]] auto CollisionDetection::boxVsBox(const Transform& transformAlpha, const Collider& colliderAlpha,
                                                const Transform& transformBeta, const Collider& colliderBeta) -> CollisionResult
{
  CollisionResult result;

  Rect aabbAlpha = colliderAlpha.getAABB(transformAlpha);
  Rect aabbBeta = colliderBeta.getAABB(transformBeta);

  if (aabbAlpha.right() < aabbBeta.left() || aabbAlpha.left() > aabbBeta.right() || aabbAlpha.bottom() < aabbBeta.top() ||
      aabbAlpha.top() > aabbBeta.bottom()) {
    return result;
  }

  f32 overlapX = math::min(aabbAlpha.right() - aabbBeta.left(), aabbBeta.right() - aabbAlpha.left());
  f32 overlapY = math::min(aabbAlpha.bottom() - aabbBeta.top(), aabbBeta.bottom() - aabbAlpha.top());

  if (overlapX < overlapY) {
    result.depth = overlapX;
    result.normal = (aabbAlpha.center().x < aabbBeta.center().x) ? Vec2(UNIT_NEGATIVE_ONE, UNIT_ZERO) : Vec2(UNIT_ONE, UNIT_ZERO);
  } else {
    result.depth = overlapY;
    result.normal = (aabbAlpha.center().y < aabbBeta.center().y) ? Vec2(UNIT_ZERO, UNIT_NEGATIVE_ONE) : Vec2(UNIT_ZERO, UNIT_ONE);
  }

  result.contactPoint = aabbAlpha.center() + ((aabbBeta.center() - aabbAlpha.center()) * HALF_FACTOR);
  result.collides = true;

  return result;
}

[[nodiscard]] auto CollisionDetection::circleVsCircle(const Transform& transformAlpha, const Collider& colliderAlpha,
                                                      const Transform& transformBeta, const Collider& colliderBeta) -> CollisionResult
{
  CollisionResult result;

  Vec2 centerAlpha = transformAlpha.position() + colliderAlpha.getOffset();
  Vec2 centerBeta = transformBeta.position() + colliderBeta.getOffset();

  f32 radiusAlpha = colliderAlpha.getRadius() * math::max(transformAlpha.scale().x, transformAlpha.scale().y);
  f32 radiusBeta = colliderBeta.getRadius() * math::max(transformBeta.scale().x, transformBeta.scale().y);

  Vec2 delta = centerBeta - centerAlpha;
  f32 distanceSquared = glm::dot(delta, delta);
  f32 radiusSum = radiusAlpha + radiusBeta;

  if (distanceSquared > (radiusSum * radiusSum)) {
    return result;
  }

  f32 distance = glm::sqrt(distanceSquared);
  result.collides = true;

  if (distance > EPSILON) {
    result.normal = delta / distance;
    result.depth = radiusSum - distance;
    result.contactPoint = centerAlpha + (result.normal * (radiusAlpha - (result.depth * HALF_FACTOR)));
  } else {
    result.normal = Vec2(UNIT_ONE, UNIT_ZERO);
    result.depth = radiusSum;
    result.contactPoint = centerAlpha;
  }

  return result;
}

[[nodiscard]] auto CollisionDetection::boxVsCircle(const Transform& transformBox, const Collider& colliderBox,
                                                   const Transform& transformCircle, const Collider& colliderCircle) -> CollisionResult
{
  CollisionResult result;

  Rect boxAABB = colliderBox.getAABB(transformBox);
  Vec2 circleCenter = transformCircle.position() + colliderCircle.getOffset();
  f32 circleRadius = colliderCircle.getRadius() * math::max(transformCircle.scale().x, transformCircle.scale().y);

  Vec2 closestPoint;
  closestPoint.x = math::max(boxAABB.left(), math::min(circleCenter.x, boxAABB.right()));
  closestPoint.y = math::max(boxAABB.top(), math::min(circleCenter.y, boxAABB.bottom()));

  Vec2 delta = circleCenter - closestPoint;
  f32 distanceSquared = glm::dot(delta, delta);

  if (distanceSquared > (circleRadius * circleRadius)) {
    return result;
  }

  result.collides = true;

  if (distanceSquared > EPSILON) {
    f32 distance = glm::sqrt(distanceSquared);
    result.normal = delta / distance;
    result.depth = circleRadius - distance;
    result.contactPoint = closestPoint;
  } else {
    f32 leftPen = circleCenter.x - boxAABB.left();
    f32 rightPen = boxAABB.right() - circleCenter.x;
    f32 topPen = circleCenter.y - boxAABB.top();
    f32 bottomPen = boxAABB.bottom() - circleCenter.y;

    f32 minPen = std::min({leftPen, rightPen, topPen, bottomPen});
    result.depth = circleRadius + minPen;

    if (minPen == leftPen) {
      result.normal = Vec2(UNIT_NEGATIVE_ONE, UNIT_ZERO);
      result.contactPoint = Vec2(boxAABB.left(), circleCenter.y);
    } else if (minPen == rightPen) {
      result.normal = Vec2(UNIT_ONE, UNIT_ZERO);
      result.contactPoint = Vec2(boxAABB.right(), circleCenter.y);
    } else if (minPen == topPen) {
      result.normal = Vec2(UNIT_ZERO, UNIT_NEGATIVE_ONE);
      result.contactPoint = Vec2(circleCenter.x, boxAABB.top());
    } else {
      result.normal = Vec2(UNIT_ZERO, UNIT_ONE);
      result.contactPoint = Vec2(circleCenter.x, boxAABB.bottom());
    }
  }

  return result;
}

[[nodiscard]] auto CollisionDetection::polygonVsPolygon(const Transform& transformAlpha, const Collider& colliderAlpha,
                                                        const Transform& transformBeta, const Collider& colliderBeta) -> CollisionResult
{
  CollisionResult result;

  auto verticesAlpha = colliderAlpha.getWorldVertices(transformAlpha);
  auto verticesBeta = colliderBeta.getWorldVertices(transformBeta);
  auto normalsAlpha = colliderAlpha.getNormals(transformAlpha);
  auto normalsBeta = colliderBeta.getNormals(transformBeta);

  std::vector<Vec2> allNormals;
  allNormals.reserve(normalsAlpha.size() + normalsBeta.size());
  allNormals.insert(allNormals.end(), normalsAlpha.begin(), normalsAlpha.end());
  allNormals.insert(allNormals.end(), normalsBeta.begin(), normalsBeta.end());

  auto [collides, satResult] = satTest(verticesAlpha, verticesBeta, allNormals);
  if (collides) {
    result = satResult;
    result.collides = true;
  }

  return result;
}

[[nodiscard]] auto CollisionDetection::edgeVsCircle(const Transform& transformEdge, const Collider& colliderEdge,
                                                    const Transform& transformCircle, const Collider& colliderCircle) -> CollisionResult
{
  CollisionResult result;

  auto [edgeStart, edgeEnd] = colliderEdge.getEdgePoints();
  edgeStart = transformEdge.position() + colliderEdge.getOffset() + edgeStart;
  edgeEnd = transformEdge.position() + colliderEdge.getOffset() + edgeEnd;

  Vec2 circleCenter = transformCircle.position() + colliderCircle.getOffset();
  f32 circleRadius = colliderCircle.getRadius() * math::max(transformCircle.scale().x, transformCircle.scale().y);

  Vec2 edgeVec = edgeEnd - edgeStart;
  f32 edgeLengthSquared = glm::dot(edgeVec, edgeVec);

  if (edgeLengthSquared < EPSILON) {
    Vec2 delta = circleCenter - edgeStart;
    f32 distance = glm::length(delta);
    if (distance <= circleRadius + EPSILON) {
      result.collides = true;
      result.depth = circleRadius - distance;
      result.normal = (distance > EPSILON) ? (delta / distance) : Vec2(UNIT_ONE, UNIT_ZERO);
      result.contactPoint = edgeStart + result.normal * (distance - (result.depth * HALF_FACTOR));
    }
    return result;
  }

  Vec2 toCircle = circleCenter - edgeStart;
  f32 tValue = glm::dot(toCircle, edgeVec) / edgeLengthSquared;
  tValue = math::clamp(tValue, 0.0F, CLAMP_MAX_VALUE);

  Vec2 closestPoint = edgeStart + tValue * edgeVec;
  Vec2 delta = circleCenter - closestPoint;
  f32 distanceSquared = glm::dot(delta, delta);

  if (distanceSquared > (circleRadius * circleRadius)) {
    return result;
  }

  result.collides = true;

  if (distanceSquared > EPSILON) {
    f32 distance = glm::sqrt(distanceSquared);
    result.normal = delta / distance;
    result.depth = circleRadius - distance;
    result.contactPoint = closestPoint + result.normal * (distance - (result.depth * HALF_FACTOR));
  } else {
    result.normal = glm::normalize(Vec2(-edgeVec.y, edgeVec.x));
    result.depth = circleRadius;
    result.contactPoint = closestPoint;
  }

  return result;
}

[[nodiscard]] auto CollisionDetection::satTest(const std::vector<Vec2>& verticesPolygonA, const std::vector<Vec2>& verticesPolygonB,
                                               const std::vector<Vec2>& axesNormals) -> std::pair<bool, CollisionResult>
{
  CollisionResult result;
  result.collides = true;
  f32 minOverlap = std::numeric_limits<f32>::max();
  Vec2 smallestAxis;

  for (const auto& normal : axesNormals) {
    auto [minA, maxA] = projectPolygon(verticesPolygonA, normal);
    auto [minB, maxB] = projectPolygon(verticesPolygonB, normal);

    if (maxA < minB || maxB < minA) {
      result.collides = false;
      return {false, result};
    }

    f32 overlap = math::min(maxA, maxB) - math::max(minA, minB);
    if (overlap < minOverlap) {
      minOverlap = overlap;
      smallestAxis = normal;
    }
  }

  result.depth = minOverlap;
  result.normal = smallestAxis;

  Vec2 centerA(0, 0);
  Vec2 centerB(0, 0);

  for (const auto& vertex : verticesPolygonA) {
    centerA += vertex;
  }

  for (const auto& vertex : verticesPolygonB) {
    centerB += vertex;
  }

  centerA /= static_cast<f32>(verticesPolygonA.size());
  centerB /= static_cast<f32>(verticesPolygonB.size());

  result.contactPoint = centerA + ((centerB - centerA) * HALF_FACTOR);

  return {true, result};
}

[[nodiscard]] auto CollisionDetection::continuousDetection(Entity entityAlpha, Entity entityBeta, World& world, f32 deltaTime)
    -> CollisionResult
{
  (void)entityAlpha;
  (void)entityBeta;
  (void)world;
  (void)deltaTime;

  CollisionResult result;
  return result;
}

[[nodiscard]] auto CollisionDetection::calculateContactPoints(const CollisionResult& result, World& world) -> std::vector<Vec2>
{
  (void)result;
  (void)world;

  return {};
}

[[nodiscard]] auto CollisionDetection::calculateManifold(const CollisionResult& result, World& world) -> CollisionResult::ContactData
{
  (void)result;
  (void)world;

  return CollisionResult::ContactData{};
}

[[nodiscard]] auto CollisionDetection::closestPointOnPolygon(const Vec2& point, const std::vector<Vec2>& polygon) -> Vec2
{
  if (polygon.empty()) {
    return point;
  }

  Vec2 closest = polygon[0];
  f32 minDistSq = glm::dot(point - closest, point - closest);

  for (usize i = ONE; i < polygon.size(); ++i) {
    f32 distSq = glm::dot(point - polygon[i], point - polygon[i]);
    if (distSq < minDistSq) {
      minDistSq = distSq;
      closest = polygon[i];
    }
  }

  return closest;
}

[[nodiscard]] auto CollisionDetection::findClosestFeatures(const Transform& transformAlpha, const Collider& colliderAlpha,
                                                           const Transform& transformBeta, const Collider& colliderBeta)
    -> std::pair<Vec2, Vec2>
{
  Rect aabbAlpha = colliderAlpha.getAABB(transformAlpha);
  Rect aabbBeta = colliderBeta.getAABB(transformBeta);

  return {aabbAlpha.center(), aabbBeta.center()};
}

auto CollisionDetection::GJK::SimplexData::add(const Vec2& point) -> void
{
  if (m_size < MAX_SIMPLEX_POINTS) {
    m_points[m_size] = point;
    m_size++;
  }
}

[[nodiscard]] auto CollisionDetection::GJK::SimplexData::containsOrigin(Vec2& direction) const -> bool
{
  static auto getDirection = [](const Vec2& pointA, const Vec2& pointB) {
    Vec2 edgeAB = pointB - pointA;
    Vec2 vectorAO = -pointA;
    Vec2 perp = Vec2(-edgeAB.y, edgeAB.x);
    if (glm::dot(perp, vectorAO) > 0) {
      return perp;
    }
    return -perp;
  };

  switch (m_size) {
    case LINE_SEGMENT_SIZE: {
      Vec2 pointA = m_points[SECOND_POINT_INDEX];
      Vec2 pointB = m_points[FIRST_POINT_INDEX];
      Vec2 edgeAB = pointB - pointA;
      Vec2 vectorAO = -pointA;

      if (glm::dot(edgeAB, vectorAO) > 0) {
        direction = getDirection(pointA, pointB);
      } else {
        direction = vectorAO;
      }
      return false;
    }
    case TRIANGLE_SIZE: {
      Vec2 pointA = m_points[THIRD_POINT_INDEX];
      Vec2 pointB = m_points[SECOND_POINT_INDEX];
      Vec2 pointC = m_points[FIRST_POINT_INDEX];

      Vec2 edgeAB = pointB - pointA;
      Vec2 edgeAC = pointC - pointA;
      Vec2 vectorAO = -pointA;

      Vec2 abPerp = Vec2(-edgeAB.y, edgeAB.x);
      Vec2 acPerp = Vec2(edgeAC.y, -edgeAC.x);

      if (glm::dot(abPerp, pointC - pointA) > 0) {
        abPerp = -abPerp;
      }
      if (glm::dot(acPerp, pointB - pointA) > 0) {
        acPerp = -acPerp;
      }

      if (glm::dot(abPerp, vectorAO) > 0) {
        direction = abPerp;
        return false;
      }

      if (glm::dot(acPerp, vectorAO) > 0) {
        direction = acPerp;
        return false;
      }

      return true;
    }
    default: return false;
  }
}

[[nodiscard]] auto CollisionDetection::GJK::test(const Transform& transformAlpha, const Collider& colliderAlpha,
                                                 const Transform& transformBeta, const Collider& colliderBeta)
    -> std::pair<bool, SimplexData>
{
  Vec2 direction(INITIAL_DIRECTION_X, INITIAL_DIRECTION_Y);
  SimplexData simplex;

  Vec2 pointA = support(transformAlpha, colliderAlpha, direction) - support(transformBeta, colliderBeta, -direction);
  simplex.add(pointA);

  direction = -pointA;

  for (int iteration = 0; iteration < static_cast<int>(MAX_GJK_ITERATIONS); ++iteration) {
    Vec2 pointB = support(transformAlpha, colliderAlpha, direction) - support(transformBeta, colliderBeta, -direction);

    if (glm::dot(pointB, direction) < 0) {
      return {false, simplex};
    }

    simplex.add(pointB);

    if (simplex.containsOrigin(direction)) {
      return {true, simplex};
    }
  }

  return {false, simplex};
}

[[nodiscard]] auto CollisionDetection::GJK::distance(const Transform& transformAlpha, const Collider& colliderAlpha,
                                                     const Transform& transformBeta, const Collider& colliderBeta)
    -> std::pair<f32, std::pair<Vec2, Vec2>>
{
  Rect aabbAlpha = colliderAlpha.getAABB(transformAlpha);
  Rect aabbBeta = colliderBeta.getAABB(transformBeta);

  Vec2 centerAlpha = aabbAlpha.center();
  Vec2 centerBeta = aabbBeta.center();

  f32 distance = glm::length(centerBeta - centerAlpha);
  return {distance, {centerAlpha, centerBeta}};
}

[[nodiscard]] auto CollisionDetection::GJK::support(const Transform& transform, const Collider& collider, const Vec2& direction) -> Vec2
{
  Rect aabb = collider.getAABB(transform);

  Vec2 result;
  result.x = (direction.x > 0) ? aabb.right() : aabb.left();
  result.y = (direction.y > 0) ? aabb.bottom() : aabb.top();

  return result;
}

[[nodiscard]] auto CollisionDetection::GJK::tripleProduct(const Vec2& vectorAlpha, const Vec2& vectorBeta, const Vec2& vectorGamma) -> Vec2
{
  f32 dotAC = glm::dot(vectorAlpha, vectorGamma);
  f32 dotBC = glm::dot(vectorBeta, vectorGamma);
  return vectorBeta * dotAC - vectorAlpha * dotBC;
}

[[nodiscard]] auto CollisionDetection::EPA::calculatePenetration(const GJK::SimplexData& simplex, const Transform& transformAlpha,
                                                                 const Collider& colliderAlpha, const Transform& transformBeta,
                                                                 const Collider& colliderBeta) -> std::pair<f32, Vec2>
{
  (void)simplex;
  (void)transformAlpha;
  (void)colliderAlpha;
  (void)transformBeta;
  (void)colliderBeta;

  return {0.0F, Vec2{}};
}

EGE_NAMESPACE_END