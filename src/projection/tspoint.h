
#ifndef TSPOINT_H_
#define TSPOINT_H_

namespace LSP {

class TSPoint
{
public:
		friend TSPoint operator+ (const TSPoint& pt, const TSPoint& c);
		friend TSPoint operator- (const TSPoint& pt, const TSPoint& c);
		friend TSPoint operator* (const TSPoint& pt, double scalar);
		friend TSPoint operator/ (const TSPoint& pt, double scalar);

        enum TSPointType {
                D2,
                D3
        };

        explicit TSPoint();
        TSPoint(double x, double y);
        TSPoint(double x, double y, double z);

        double distance(const TSPoint& point) const;

        double getX() const;
        double getY() const;
        double getZ() const;
        void setX(double x);
        void setY(double y);
        void setZ(double z);

private:
        TSPointType type;
        double x;
        double y;
        double z;
};

TSPoint operator+ (const TSPoint& pt, const TSPoint& c);
TSPoint operator- (const TSPoint& pt, const TSPoint& c);
TSPoint operator* (const TSPoint& pt, double scalar);
TSPoint operator/ (const TSPoint& pt, double scalar);

} // namespace LSP

#endif /* TSPOINT_H_ */
