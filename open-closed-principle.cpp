#include <string>
#include <vector>
#include <iostream>

template <typename T> struct Specification;
// template <typename T> struct AndSpecification : public Specification<T>;

enum class Color { red, green, blue };
enum class Size { small, medium, large };

struct Product
{
    std::string name;
    Color color;
    Size size;
};

struct ProductFilter
{
    virtual ~ProductFilter() = default;

    static std::vector<Product*> by_color(const std::vector<Product*> products, Color color)
    {
        std::vector<Product*> result;
        for (auto& p : products)
            if (p->color == color)
                result.push_back(p);
        return result;
    }

    static std::vector<Product*> by_size(const std::vector<Product*> products, Size size)
    {
        std::vector<Product*> result;
        for (auto& p : products)
            if (p->size == size)
                result.push_back(p);
        return result;
    }

    static std::vector<Product*> by_size_and_color(const std::vector<Product*> products, Size size, Color color)
    {
        std::vector<Product*> result;
        for (auto& p : products)
            if (p->size == size && p->color == color)
                result.push_back(p);
        return result;
    }
};

template <typename T> struct AndSpecification : public Specification<T>
{
    const Specification<T>& left;
    const Specification<T>& right;

    AndSpecification(const Specification<T>& left, const Specification<T>& right)
        : left(left), right(right)
    {}

    bool is_satisfied(const T& product) const override
    {
        return left.is_satisfied(product) && right.is_satisfied(product);
    }
};

template <typename T> struct OrSpecification : public Specification<T>
{
    const Specification<T>& left;
    const Specification<T>& right;

    OrSpecification(const Specification<T>& left, const Specification<T>& right)
        : left(left), right(right)
    {}

    bool is_satisfied(const T& product) const override
    {
        return left.is_satisfied(product) || right.is_satisfied(product);
    }
};

template <typename T> struct Specification
{
    virtual bool is_satisfied(const T&) const = 0;

    AndSpecification<T> operator&&(const Specification& other) const
    {
        return AndSpecification<T>(*this, other);
    }

    OrSpecification<T> operator||(const Specification& other) const
    {
        return OrSpecification<T>(*this, other);
    }
};

template <typename T> struct Filter
{
    virtual std::vector<T*> by_specification(const std::vector<T*> items, Specification<T>& specification) const
    {
        std::vector<T*> result;
        for (auto& item : items)
            if (specification.is_satisfied(*item))
                result.push_back(item);
        return result;
    }
};

struct ProductColorSpecification : Specification<Product>
{
    Color color;
    ProductColorSpecification(Color color) : color(color) {}
    bool is_satisfied(const Product& product) const override
    {
        return product.color == color;
    }
};

struct ProductSizeSpecification : Specification<Product>
{
    Size size;
    ProductSizeSpecification(Size size) : size(size) {}
    bool is_satisfied(const Product& product) const override
    {
        return product.size == size;
    }
};

struct BetterProductFilter : Filter<Product>
{
    std::vector<Product*> by_specification(const std::vector<Product*> items, Specification<Product>& specification) const override
    {
        return Filter<Product>::by_specification(items, specification);
    }
};

struct Meat : Product
{
    float weight;
};

struct MeatWeightSpecification : Specification<Meat>
{
    float weight;
    MeatWeightSpecification(float weight) : weight(weight) {}
    bool is_satisfied(const Meat& meat) const override
    {
        return abs(meat.weight - weight) < 1;
    }
};

int main()
{
    Product bread{"Bread", Color::red, Size::small};
    Product milk{"Milk", Color::green, Size::small};
    Product water{"Water", Color::blue, Size::large};
    Product coffee{"Coffee", Color::green, Size::medium};
    Product tea{"Tea", Color::green, Size::small};
    Product juice{"Juice", Color::blue, Size::medium};
    Product cherry{"Cherries", Color::red, Size::large};
    std::vector<Product*> products{&bread, &milk, &water, &coffee, &tea, &juice, &cherry};

    std::cout << "Filtering without OCP..." << std::endl;
    std::vector<Product*> green_products = ProductFilter::by_color(products, Color::green);
    for (Product* p : green_products)
        std::cout << p->name << " is green." << std::endl;

    std::vector<Product*> large_products = ProductFilter::by_size(products, Size::large);
    for (Product* p : large_products)
        std::cout << p->name << " is large." << std::endl;

    std::vector<Product*> green_large_products = ProductFilter::by_size_and_color(products, Size::small, Color::green);
    for (Product* p : green_large_products)
        std::cout << p->name << " is green and large." << std::endl;

    std::cout << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    std::cout << "Filtering with OCP..." << std::endl;
    BetterProductFilter bf;
    ProductColorSpecification productColorSpecification = ProductColorSpecification(Color::green);
    green_products = bf.by_specification(products, productColorSpecification);
    for (Product* p : green_products)
        std::cout << p->name << " is green." << std::endl;

    ProductSizeSpecification productSizeSpecification = ProductSizeSpecification(Size::large);
    large_products = bf.by_specification(products, productSizeSpecification);
    for (Product* p : large_products)
        std::cout << p->name << " is large." << std::endl;
    
    AndSpecification<Product> productSizeAndColorSpecification = ProductSizeSpecification(Size::small) && ProductColorSpecification(Color::green);
    green_large_products = bf.by_specification(products, productSizeAndColorSpecification);
    for (Product* p : green_large_products)
        std::cout << p->name << " is green and large." << std::endl;

    Meat chicken{"Chicken", Color::red, Size::small, 10};
    Meat beef{"Beef", Color::red, Size::medium, 20};
    Meat lamb{"Lamb", Color::red, Size::large, 30};
    products.push_back(&chicken);

    OrSpecification<Product> productSizeOrColorSpecification = ProductColorSpecification(Color::red) || (ProductSizeSpecification(Size::large) && ProductColorSpecification(Color::blue));
    std::vector<Product*> red_or_large_and_blue = bf.by_specification(products, productSizeOrColorSpecification);
    for (Product* p : red_or_large_and_blue)
        std::cout << p->name << " is red or large and blue" << std::endl;

    //@TODO: Figure out how to filter through a mix of products and derived products.
    // OrSpecification<Meat> productWeightOrSizeSpecification = MeatWeightSpecification(20) || MeatWeightSpecification(15);
    // std::vector<Meat*> meats = bf.by_specification(products, productWeightOrSizeSpecification);

    return 0;
}