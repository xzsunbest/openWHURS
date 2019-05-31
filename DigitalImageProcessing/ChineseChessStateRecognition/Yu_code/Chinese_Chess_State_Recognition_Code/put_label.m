function put_label(x, y, label, color)
    if label
        for dx = 0:1
            for dy = 0:1
                h = text(x+dx, y+dy, label);
                set(h, 'Color', color, 'FontSize', 10); 
            end % dy
        end % dx
        h = text(x, y, label);
        set(h, 'Color', color, 'FontSize', 10);
    end
end